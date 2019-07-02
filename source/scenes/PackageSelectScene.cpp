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
#include "../NetManager.hpp"
#include "../SceneDirector.hpp"

using namespace ku;
using namespace ku::models;
using namespace ku::views;
using namespace std;
using namespace std::placeholders;

namespace ku::scenes {
    PackageSelectScene::PackageSelectScene() {
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

        addSubView(_headerView);
        addSubView(_updateView);
        addSubView(_statusView);
        addSubView(_installRowView);
        addSubView(_footerView);

        _showUpdateView();
        _versionRequest = NetManager::getLatestKosmosVersion();
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

        if (_versionRequest != NULL)
            delete _versionRequest;
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
        if (_versionRequest != NULL) {
            _updateVersionRequest();
        }

        Scene::render(rect, dTime);
    }

    void PackageSelectScene::_updateVersionRequest() {
        mutexLock(&_versionRequest->mutexRequest);

        _updateView->setProgress(_versionRequest->progress);
        if (_versionRequest->isComplete) {
            _latestVersion = string(_versionRequest->getData());

            _showPackageSelectViews();

            delete _versionRequest;
            _versionRequest = NULL;
        }
        else if (_versionRequest->hasError) {
            _showStatusView(_versionRequest->errorMessage, "Please restart the app to try again.");

            delete _versionRequest;
            _versionRequest = NULL;
        }

        if (_versionRequest != NULL)
            mutexUnlock(&_versionRequest->mutexRequest);
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
        _updateView->hidden = true;
        _statusView->hidden = true;

        _installRowView->hidden = false;
        _installRowView->hasFocus = true;

        string version = ConfigManager::getCurrentVersion();
        if (version.compare(_latestVersion) == 0) {
            _installRowView->setPrimaryText("Reinstall Kosmos");
        } else {
            _installRowView->setPrimaryText("Install Latest Kosmos");
        }

        if (version == "" || version.compare(_latestVersion) == 0) {
            _installRowView->setSecondaryText("Latest Version is " + _latestVersion);
        } else {
            _installRowView->setSecondaryText("You currently have version " + version + " installed, and the latest version is " + _latestVersion + ".");
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
}