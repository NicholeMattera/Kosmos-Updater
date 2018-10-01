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

#include "AppUpdateScene.hpp"
#include "../SceneDirector.hpp"
#include "../NetManager.hpp"

AppUpdateScene::AppUpdateScene() {
    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    _updateView = new UpdateView("Checking for updates to SDFiles Updater...");
    _updateView->frame.x = 0;
    _updateView->frame.y = 200;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    addSubView(_headerView);
    addSubView(_updateView);
    addSubView(_footerView);

    _versionRequest = NetManager::getLatestAppVersion();
}

AppUpdateScene::~AppUpdateScene() {
    if (_versionRequest != NULL)
        delete _versionRequest;

    if (_appRequest != NULL)
        delete _appRequest;

    if (_headerView != NULL)
        delete _headerView;

    if (_updateView != NULL)
        delete _updateView;

    if (_footerView != NULL)
        delete _footerView;
}

void AppUpdateScene::handleButton(u32 buttons) {}

void AppUpdateScene::render(SDL_Rect rect, double dTime) {
    // App Version Downloading
    if (_versionRequest != NULL) {
        mutexLock(&_versionRequest->mutexRequest);

        // Success
        if (_versionRequest->isComplete) {
            if (string(VERSION).compare(_versionRequest->data) == 0) {
                SceneDirector::currentScene = SCENE_PACKAGE_SELECT;
            } else {
                _updateView->setProgress(0);
                _updateView->setText("Getting the latest version of SDFiles Updater...");

                delete _versionRequest;
                _versionRequest = NULL;

                _appRequest = NetManager::getLatestApp();
            }
        }
        // Failure
        else if (_versionRequest->hasError) {
            // TODO: Handle Error.
            printf("Version Error: %s", _versionRequest->errorMessage.c_str());

            delete _versionRequest;
            _versionRequest = NULL;
        }
        // Progress Update
        else {
            _updateView->setProgress(_versionRequest->progress);
        }

        if (_versionRequest != NULL)
            mutexUnlock(&_versionRequest->mutexRequest);
    }
    
    // App Downloading
    else if (_appRequest != NULL) {
        mutexLock(&_appRequest->mutexRequest);
        
        // Success
        if (_appRequest->isComplete) {
            // TODO: Handle new application.
        }
        // Failure
        else if (_appRequest->hasError) {
            // TODO: Handle Error.
            printf("App Error: %s", _appRequest->errorMessage.c_str());

            delete _appRequest;
            _appRequest = NULL;
        }
        // Progress Update
        else {
            _updateView->setProgress(_appRequest->progress);
        }

        if (_appRequest != NULL)
            mutexUnlock(&_appRequest->mutexRequest);
    }

    Scene::render(rect, dTime);
}
