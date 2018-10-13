// SDFiles Updater
// Copyright (C) 2018 Steven Mattera
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

#include "PackageDownloadScene.hpp"
#include "../SceneDirector.hpp"
#include "../NetManager.hpp"
#include "../ConfigManager.hpp"
#include "../FileManager.hpp"

PackageDownloadScene::PackageDownloadScene() {
    _headerView = new HeaderView("SDFiles Updater", true);
    _headerView->frame = { 0, 0, 1280, 88 };

    _updateView = new UpdateView("Downloading the latest SDFiles...");
    _updateView->frame.x = 0;
    _updateView->frame.y = 200;

    _statusView = new StatusView("", "");
    _statusView->frame.x = 0;
    _statusView->frame.y = 323;
    _statusView->hidden = true;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    addSubView(_headerView);
    addSubView(_updateView);
    addSubView(_statusView);
    addSubView(_footerView);

    string bundle = ConfigManager::getBundle();
    string channel = ConfigManager::getChannel();
    _packageRequest = NetManager::getLatestSDFiles(bundle, channel);
    _packageUnzip = NULL;
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

    if (_packageRequest != NULL)
        delete _packageRequest;

    if (_packageUnzip != NULL)
        delete _packageUnzip;
}

void PackageDownloadScene::handleButton(u32 buttons) {
    if (!_statusView->hidden && buttons & KEY_A) {
        SceneDirector::exitApp = true;
    }
}

void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
    if (_packageRequest != NULL) {
        _updatePackageRequest();
    }
    else if (_packageUnzip != NULL) {
        _updatePackageUnzip();
    }

    Scene::render(rect, dTime);
}

void PackageDownloadScene::_updatePackageRequest() {
    mutexLock(&_packageRequest->mutexRequest);

    _updateView->setProgress(_packageRequest->progress);
    if (_packageRequest->isComplete) {
        FileManager::writeFile("temp.zip", _packageRequest);

        _showStatus("SD Files has been updated to version 9.0.4!", "Please restart your Switch to run the latest SD Files.");
        
        // _updateView->setText("Extracting the latest SDFiles...");
        // _updateView->setProgress(0);

        // _packageUnzip = FileManager::unzipArchive("temp.zip", "sdmc:");

        delete _packageRequest;
        _packageRequest = NULL;
    }
    else if (_packageRequest->hasError) {
        _showStatus(_packageRequest->errorMessage, "Please restart the app to try again.");

        delete _packageRequest;
        _packageRequest = NULL;
    }

    if (_packageRequest != NULL)
        mutexUnlock(&_packageRequest->mutexRequest);
}

void PackageDownloadScene::_updatePackageUnzip() {
    mutexLock(&_packageUnzip->mutexRequest);

    _updateView->setProgress(_packageUnzip->progress);
    if (_packageUnzip->isComplete) {
        // TODO: Delete temp.zip

        _showStatus("SD Files has been updated to version 9.0.4!", "Please restart your Switch to run the latest SD Files.");

        delete _packageUnzip;
        _packageUnzip = NULL;
    }
    else if (_packageUnzip->hasError) {
        // TODO: Delete temp.zip

        _showStatus(_packageUnzip->errorMessage, "Please restart the app to try again.");

        delete _packageUnzip;
        _packageUnzip = NULL;
    }

    if (_packageUnzip != NULL)
        mutexUnlock(&_packageUnzip->mutexRequest);
}

void PackageDownloadScene::_showStatus(string text, string subtext) {
    _statusView->setText(text);
    _statusView->setSubtext(subtext);

    _updateView->hidden = true;
    _statusView->hidden = false;

    _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
}
