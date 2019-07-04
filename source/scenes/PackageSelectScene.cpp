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

#include "PackageSelectScene.hpp"

#include "../ConfigManager.hpp"
#include "../SceneDirector.hpp"

using namespace ku;
using namespace ku::models;
using namespace ku::views;
using namespace std;
using namespace std::placeholders;
using namespace swurl;

namespace ku::scenes {
    PackageSelectScene::PackageSelectScene() {
        SessionManager::onProgressChanged = bind(&PackageSelectScene::_onProgressUpdate, this, _1, _2);
        SessionManager::onCompleted = bind(&PackageSelectScene::_onCompleted, this, _1);
        SessionManager::onError = bind(&PackageSelectScene::_onError, this, _1, _2);

        _headerView = new HeaderView("Kosmos Updater", true);
        _headerView->frame = { 0, 0, 1280, 88 };

        _updateView = new UpdateView("Checking for updates to Kosmos...");
        _updateView->frame.x = 0;
        _updateView->frame.y = 200;

        _statusView = new StatusView("", "");
        _statusView->frame.x = 0;
        _statusView->frame.y = 323;

        _installRowView = new ListRowView("Install Latest Kosmos", "", SUBTITLE);
        _installRowView->frame.x = 215;
        _installRowView->frame.y = 137;
        _installRowView->isLast = true;
        _installRowView->hasFocus = true;

        _footerView = new FooterView();
        _footerView->frame = { 0, 647, 1280, 73 };

        vector<string> buttons;
        buttons.push_back("Yes");
        buttons.push_back("No");
        _ignoreConfigsAlertView = new AlertView("Ignore Config Files?", "Would you like for Kosmos Updater to ignore config\nfiles? This will prevent Kosmos Updater from overwriting\nall config files except for Hekate's main config file.", buttons);
        _ignoreConfigsAlertView->onDismiss = bind(&PackageSelectScene::_onAlertViewDismiss, this, _1, _2);

        addSubView(_headerView);
        addSubView(_updateView);
        addSubView(_statusView);
        addSubView(_installRowView);
        addSubView(_footerView);

        _showUpdateView();
    }

    PackageSelectScene::~PackageSelectScene() {
        if (_headerView != NULL)
            delete _headerView;

        if (_updateView != NULL)
            delete _updateView;

        if (_statusView != NULL)
            delete _statusView;

        if (_installRowView != NULL)
            delete _installRowView;

        if (_footerView != NULL)
            delete _footerView;

        if (_kosmosVersionRequest != NULL)
            delete _kosmosVersionRequest;
    }

    void PackageSelectScene::handleButton(u32 buttons, double dTime) {
        if (!_statusView->hidden && buttons & KEY_A) {
            SceneDirector::exitApp = true;
        }
        else if (_updateView->hidden && _statusView->hidden) {
            if (buttons & KEY_A) {
                SceneDirector::currentScene = SCENE_PACKAGE_DOWNLOAD;
            }

            if (buttons & KEY_B) {
                SceneDirector::exitApp = true;
            }
        }
    }

    void PackageSelectScene::render(SDL_Rect rect, double dTime) {
        if (_kosmosVersionRequest == NULL) {
            _kosmosVersionRequest = new WebRequest(ConfigManager::getHost() + "/" + API_VERSION + "/package/version-number");
            SessionManager::makeRequest(_kosmosVersionRequest);
        }

        Scene::render(rect, dTime);
    }

    void PackageSelectScene::_showUpdateView() {
        _updateView->setProgress(0);
        _updateView->hidden = false;
        _statusView->hidden = true;
        _installRowView->hidden = true;

        for (auto const& action : _footerView->actions) {
            delete action;
        }
        _footerView->actions.clear();
    }

    void PackageSelectScene::_showPackageSelectViews() {
        if (!ConfigManager::getReceivedIgnoreConfigWarning()) {
            _ignoreConfigsAlertView->show();
        }

        _updateView->hidden = true;
        _statusView->hidden = true;

        _installRowView->hidden = false;
        _installRowView->hasFocus = true;

        string version = ConfigManager::getCurrentVersion();
        string latestVersion = (_kosmosVersionRequest != NULL) ? _kosmosVersionRequest->response.rawResponseBody : "";
        if (version.compare(latestVersion) == 0) {
            _installRowView->setPrimaryText("Reinstall Kosmos");
        } else {
            _installRowView->setPrimaryText("Install Latest Kosmos");
        }

        if (version == "" || version.compare(latestVersion) == 0) {
            _installRowView->setSecondaryText("Latest Version is " + latestVersion);
        } else {
            _installRowView->setSecondaryText("You currently have version " + version + " installed, and the latest version is " + latestVersion + ".");
        }

        for (auto const& action : _footerView->actions) {
            delete action;
        }
        _footerView->actions.clear();
        _footerView->actions.push_back(new Action(A_BUTTON, "OK"));
        _footerView->actions.push_back(new Action(B_BUTTON, "Quit"));
    }

    void PackageSelectScene::_showStatusView(string text, string subtext) {
        _statusView->setText(text);
        _statusView->setSubtext(subtext);

        _updateView->hidden = true;
        _statusView->hidden = false;
        _installRowView->hidden = true;

        for (auto const& action : _footerView->actions) {
            delete action;
        }
        _footerView->actions.clear();
        _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
    }

    // Alert View Callback Method

    void PackageSelectScene::_onAlertViewDismiss(ModalView * view, bool success) {
        if (success && _ignoreConfigsAlertView->getSelectedOption() == 0) {
            vector<string> files;
            files.push_back("sdmc:/atmosphere/BCT.ini");
            files.push_back("sdmc:/atmosphere/loader.ini");
            files.push_back("sdmc:/atmosphere/system_settings.ini");
            files.push_back("sdmc:/bootloader/patches.ini");
            files.push_back("sdmc:/config/hid_mitm/config.ini");
            files.push_back("sdmc:/config/sys-clk/config.ini");
            files.push_back("sdmc:/config/ftpd/config.ini");
            files.push_back("sdmc:/ftpd/config.ini");
            files.push_back("sdmc:/switch/KosmosToolbox/config.json");
            files.push_back("sdmc:/switch/KosmosUpdater/internal.db");
            files.push_back("sdmc:/switch/KosmosUpdater/settings.cfg");
            ConfigManager::setFilesToIgnore(files);
        }

        ConfigManager::setReceivedIgnoreConfigWarning(true);
    }

    // Swurl Callback Methods

    void PackageSelectScene::_onProgressUpdate(WebRequest * request, double progress) {
        _updateView->setProgress(progress);
        SceneDirector::currentSceneDirector->render();
    }

    void PackageSelectScene::_onCompleted(WebRequest * request) {
        _showPackageSelectViews();
    }

    void PackageSelectScene::_onError(WebRequest * request, string error) {
        _showStatusView(error, "Please restart the app to try again.");
    }
}