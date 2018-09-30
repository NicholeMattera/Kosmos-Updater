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

PackageDownloadScene::PackageDownloadScene() {
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

    _statusTextView = new TextView(AssetManager::subbody_font, "Downloading the latest SDFiles...", AssetManager::text);
    _statusTextView->frame = { 0, 491, 1280, 0 };
    _statusTextView->textAlignment = CENTER_ALIGN;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    addSubView(_headerView);
    addSubView(_downloadImageView);
    addSubView(_progressBarView);
    addSubView(_statusTextView);
    addSubView(_footerView);
}

PackageDownloadScene::~PackageDownloadScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_downloadImageView != NULL)
        delete _downloadImageView;

    if (_progressBarView != NULL)
        delete _progressBarView;

    if (_statusTextView != NULL)
        delete _statusTextView;

    if (_footerView != NULL)
        delete _footerView;
}

void PackageDownloadScene::handleButton(u32 buttons) {}

void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
    // Temporary to simulate downloading.
    _downloadProgress += 0.01;
    _progressBarView->progress = _downloadProgress;
    if (_downloadProgress >= 1) {
        SceneDirector::currentScene = SCENE_ALL_DONE;
    }

    Scene::render(rect, dTime);
}
