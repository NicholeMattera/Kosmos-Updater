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

AppUpdateScene::AppUpdateScene() {
    _downloadProgress = 0;

    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    if (AssetManager::downloading == NULL) {
        AssetManager::downloading = AssetManager::loadAsset("downloading.png");
    }
    _downloadImageView = new ImageView(AssetManager::downloading);
    _downloadImageView->frame = { 400, 200, 479, 197 };

    _progressBarView = new ProgressBarView();
    _progressBarView->frame = { 437, 457, 411, 10 };

    _statusTextView = new TextView(AssetManager::subbody_font, "Checking for updates to SDFiles Updater...", AssetManager::text);
    _statusTextView->frame = { 0, 491, 1280, 0 };
    _statusTextView->textAlignment = CENTER_ALIGN;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };
    _footerView->actions.push_back(new Action(A_BUTTON, "Package Select"));

    addSubView(_headerView);
    addSubView(_downloadImageView);
    addSubView(_progressBarView);
    addSubView(_statusTextView);
    addSubView(_footerView);
}

AppUpdateScene::~AppUpdateScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_downloadImageView != NULL)
        delete _downloadImageView;

    if (_footerView != NULL)
        delete _footerView;
}

void AppUpdateScene::handleButton(u32 buttons) {
    if (buttons & KEY_A) {
        SceneDirector::currentScene = SCENE_PACKAGE_SELECT;
    }
    
    if (buttons & KEY_X) {
        _downloadProgress++;
        _progressBarView->progress = _downloadProgress;
    }
    
    if (buttons & KEY_B) {
        _downloadProgress--;
        _progressBarView->progress = _downloadProgress;
    }
}

void AppUpdateScene::render(SDL_Rect rect, double dTime) {
    Scene::render(rect, dTime);
}
