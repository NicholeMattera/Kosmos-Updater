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
    bpcInitialize();
    
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
    
    _packageRequest = NetManager::getLatestSDFiles(bundle, channel, "zip");
    _packageExtract = NULL;
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

    if (_packageExtract != NULL)
        delete _packageExtract;

    bpcExit();
}

void PackageDownloadScene::handleButton(u32 buttons) {
    if (!_statusView->hidden && buttons & KEY_A) {
        SceneDirector::exitApp = true;
    }

    if (!_statusView->hidden && _footerView->actions.size() == 2 && buttons & KEY_X) {
        bpcRebootSystem();
    }
}

void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
    if (_packageRequest != NULL) {
        _updatePackageRequest();
    }
    else if (_packageExtract != NULL) {
        _updatePackageExtract();
    }

    Scene::render(rect, dTime);
}

void PackageDownloadScene::_updatePackageRequest() {
    mutexLock(&_packageRequest->mutexRequest);

    _updateView->setProgress(_packageRequest->progress);
    if (_packageRequest->isComplete) {
        FileManager::writeFile("temp.zip", _packageRequest);
        _versionNumber = _packageRequest->getVersionNumber();
        string numberOfFiles = _packageRequest->getNumberOfFiles();

        delete _packageRequest;
        _packageRequest = NULL;

        _updateView->setText("Extracting the latest SDFiles...");
        _updateView->setProgress(0);

        _packageExtract = new Zip("temp.zip", "sdmc:/", stoi(numberOfFiles));
        FileManager::extract(_packageExtract);
    }
    else if (_packageRequest->hasError) {
        _showStatus(_packageRequest->errorMessage, "Please restart the app to try again.", false);

        delete _packageRequest;
        _packageRequest = NULL;
    }

    if (_packageRequest != NULL)
        mutexUnlock(&_packageRequest->mutexRequest);
}

void PackageDownloadScene::_updatePackageExtract() {
    mutexLock(&_packageExtract->mutexRequest);

    _updateView->setProgress(_packageExtract->progress);
    if (_packageExtract->isComplete) {
        delete _packageExtract;
        _packageExtract = NULL;

        FileManager::deleteFile("temp.zip");
        ConfigManager::setCurrentVersion(_versionNumber);

        _showStatus("SD Files has been updated to version " + _versionNumber + "!", "Please restart your Switch to run the latest SD Files.", true);
    }
    else if (_packageExtract->hasError) {
        FileManager::deleteFile("temp.zip");

        _showStatus(_packageExtract->errorMessage, "Please restart the app to try again.", false);

        delete _packageExtract;
        _packageExtract = NULL;
    }

    if (_packageExtract != NULL)
        mutexUnlock(&_packageExtract->mutexRequest);
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
