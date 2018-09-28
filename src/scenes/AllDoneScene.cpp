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

#include "AllDoneScene.hpp"
#include "../SceneDirector.hpp"
#include "../AssetManager.hpp"

AllDoneScene::AllDoneScene() {
    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    _textView = new TextView(AssetManager::body_font, "SD Files has been updated to version 9.0.4!", AssetManager::text);
    _textView->frame = { 0, 323, 1280, 1 };
    _textView->textAlignment = CENTER_ALIGN;

    _subtextView = new TextView(AssetManager::subbody_font, "Please restart your Switch to run the latest SD Files.", AssetManager::text);
    _subtextView->frame = { 0, 375, 1280, 1 };
    _subtextView->textAlignment = CENTER_ALIGN;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };
    _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));

    addSubView(_headerView);
    addSubView(_textView);
    addSubView(_subtextView);
    addSubView(_footerView);
}

AllDoneScene::~AllDoneScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_textView != NULL)
        delete _textView;

    if (_subtextView != NULL)
        delete _subtextView;

    if (_footerView != NULL)
        delete _footerView;
}

void AllDoneScene::handleButton(u32 buttons) {
    if (buttons & KEY_A) {
        SceneDirector::exitApp = true;
    }
}

void AllDoneScene::render(SDL_Rect rect, double dTime) {
    Scene::render(rect, dTime);
}
