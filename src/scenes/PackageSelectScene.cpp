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

#include "PackageSelectScene.hpp"

PackageSelectScene::PackageSelectScene() {
    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    addSubView(_headerView);
    addSubView(_footerView);
}

PackageSelectScene::~PackageSelectScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_footerView != NULL)
        delete _footerView;
}

void PackageSelectScene::handleButton(u32 buttons) {
    
}

void PackageSelectScene::render(SDL_Rect rect, double dTime) {
    Scene::render(rect, dTime);
}
