// Kosmos Updater
// Copyright (C) 2019 Steven Mattera
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <Trim.hpp>

#include "PackageDownloadScene.hpp"

#include "../ConfigManager.hpp"
#include "../FileManager.hpp"
#include "../SceneDirector.hpp"

using namespace ku;
using namespace ku::models;
using namespace ku::views;
using namespace std;
using namespace std::placeholders;
using namespace swurl;

namespace ku::scenes {
    PackageDownloadScene::PackageDownloadScene() {
        SessionManager::onProgressChanged = bind(&PackageDownloadScene::_onProgressUpdate, this, _1, _2);
        SessionManager::onCompleted = bind(&PackageDownloadScene::_onCompleted, this, _1);
        SessionManager::onError = bind(&PackageDownloadScene::_onError, this, _1, _2);

        bpcInitialize();
        
        _headerView = new HeaderView("Kosmos Updater", true);
        _headerView->frame = { 0, 0, 1280, 88 };

        _updateView = new UpdateView("Downloading the latest Kosmos...");
        _updateView->frame.x = 0;
        _updateView->frame.y = 200;

        _statusView = new StatusView("", "");
        _statusView->frame.x = 0;
        _statusView->frame.y = 323;
        _statusView->hidden = true;

        _footerView = new FooterView();
        _footerView->frame = { 0, 647, 1280, 73 };

        vector<string> buttons;
        buttons.push_back("Restart");
        buttons.push_back("Quit");
        _restartAlertView = new AlertView("Are you sure?", "Restarting while using an ExFAT formatted SD card\nwill cause it to corrupt. It is recommended that if\nyou have an ExFAT formatted SD card to quit,\npress the home button, and restart the switch.", buttons);
        _restartAlertView->onDismiss = bind(&PackageDownloadScene::_onAlertViewDismiss, this, _1, _2);

        addSubView(_headerView);
        addSubView(_updateView);
        addSubView(_statusView);
        addSubView(_footerView);
    }

    PackageDownloadScene::~PackageDownloadScene() {
        if (_headerView != NULL)
            delete _headerView;

        if (_updateView != NULL)
            delete _updateView;

        if (_statusView != NULL)
            delete _statusView;

        if (_footerView != NULL)
            delete _footerView;

        if (_kosmosRequest != NULL)
            delete _kosmosRequest;

        bpcExit();
    }

    void PackageDownloadScene::handleButton(u32 buttons, double dTime) {
        if (!_statusView->hidden && buttons & KEY_A) {
            SceneDirector::exitApp = true;
        }

        if (!_statusView->hidden && _footerView->actions.size() == 2 && buttons & KEY_X) {
            _restartAlertView->reset();
            _restartAlertView->show();
        }
    }

    void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
        if (_kosmosRequest == NULL) {
            _kosmosRequest = new WebRequest(ConfigManager::getHost() + "/" + API_VERSION + "/package");
            SessionManager::makeRequest(_kosmosRequest);
        }

        Scene::render(rect, dTime);
    }

    void PackageDownloadScene::_showStatus(string text, string subtext, bool wasSuccessful) {
        _statusView->setText(text);
        _statusView->setSubtext(subtext);

        _updateView->hidden = true;
        _statusView->hidden = false;

        _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
        if (wasSuccessful) {
            _footerView->actions.push_back(new Action(X_BUTTON, "Restart"));
        }
    }

    void PackageDownloadScene::_onAlertViewDismiss(ModalView * view, bool success) {
        if (success) {
            if (_restartAlertView->getSelectedOption() == 0) {
                bpcRebootSystem();
            }
            else {
                SceneDirector::exitApp = true;
            }
        }
    }

    void PackageDownloadScene::_onProgressUpdate(WebRequest * request, double progress) {
        _updateView->setProgress(progress);
        SceneDirector::currentSceneDirector->render();
    }

    void PackageDownloadScene::_onCompleted(WebRequest * request) {
        FileManager::writeFile("temp.zip", request->response.rawResponseBody);
        auto versionNumber = request->response.headers.find("X-Version-Number")->second;
        rtrim(versionNumber);

        _updateView->setText("Removing old package...");
        _updateView->setProgressBarHidden(true);
        SceneDirector::currentSceneDirector->render();

        FileManager::cleanUpFiles();

        _updateView->setText("Extracting the latest package...");
        SceneDirector::currentSceneDirector->render();

        if (!FileManager::extract("temp.zip", "sdmc:/")) {
            FileManager::deleteFile("temp.zip");
            _showStatus("There was an error while trying to extract files.", "Please restart the app to try again.", false);
            return;
        }

        FileManager::deleteFile("temp.zip");
        ConfigManager::setCurrentVersion(versionNumber);

        _updateView->setText("Applying disabled game cart option...");
        SceneDirector::currentSceneDirector->render();

        FileManager::applyNoGC();

        _showStatus("Kosmos has been updated to version " + versionNumber.substr() + "!", "Please restart your Switch to finish the update.", true);
    }

    void PackageDownloadScene::_onError(WebRequest * request, string error) {
        _showStatus(error, "Please restart the app to try again.", false);
    }
}
