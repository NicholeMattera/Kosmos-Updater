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

PackageDownloadScene::PackageDownloadScene() {
    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    _updateView = new UpdateView("Downloading the latest SDFiles...");
    _updateView->frame.x = 0;
    _updateView->frame.y = 200;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    addSubView(_headerView);
    addSubView(_updateView);
    addSubView(_footerView);
}

PackageDownloadScene::~PackageDownloadScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_updateView != NULL)
        delete _updateView;

    if (_footerView != NULL)
        delete _footerView;
}

void PackageDownloadScene::handleButton(u32 buttons) {}

void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
    Scene::render(rect, dTime);
}

void PackageDownloadScene::_getProgress(double progress) {
    _updateView->setProgress(progress);
}

