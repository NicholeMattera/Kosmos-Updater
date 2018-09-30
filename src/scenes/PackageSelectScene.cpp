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
#include "../SceneDirector.hpp"
#include "../ConfigManager.hpp"

PackageSelectScene::PackageSelectScene() {
    _focusSelection = 0;

    _headerView = new HeaderView();
    _headerView->frame = { 0, 0, 1280, 88 };

    _installRowView = new ListRowView("Install Latest SDFiles", "", DEFAULT);
    _installRowView->frame.x = 215;
    _installRowView->frame.y = 137;
    _installRowView->isLast = true;
    _installRowView->hasFocus = true;

    _settingHeaderView = new ListHeaderView("Settings");
    _settingHeaderView->frame.x = 220;
    _settingHeaderView->frame.y = 208;

    string channel = ConfigManager::getChannel();
    if (channel == "bleeding-edge") {
        channel = "Bleeding Edge";
    } else {
        channel = "Stable";
    }

    _channelRowView = new ListRowView("Channel", channel, VALUE);
    _channelRowView->frame.x = 215;
    _channelRowView->frame.y = 288;

    string bundle = ConfigManager::getChannel();
    if (bundle == "hekate") {
        bundle = "Hekate";
    } else if (bundle == "atmosphere") {
        bundle = "Atmosphere";
    } else if (bundle == "reinx") {
        bundle = "ReiNX";
    } else {
        bundle = "SDFiles";
    }

    _bundleRowView = new ListRowView("Bundle", bundle, VALUE);
    _bundleRowView->frame.x = 215;
    _bundleRowView->frame.y = 359;
    _bundleRowView->isLast = true;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };
    _footerView->actions.push_back(new Action(A_BUTTON, "OK"));
    _footerView->actions.push_back(new Action(B_BUTTON, "Quit"));

    addSubView(_headerView);
    addSubView(_installRowView);
    addSubView(_settingHeaderView);
    addSubView(_channelRowView);
    addSubView(_bundleRowView);
    addSubView(_footerView);
}

PackageSelectScene::~PackageSelectScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_installRowView != NULL)
        delete _installRowView;

    if (_settingHeaderView != NULL)
        delete _settingHeaderView;

    if (_channelRowView != NULL)
        delete _channelRowView;

    if (_bundleRowView != NULL)
        delete _bundleRowView;

    if (_footerView != NULL)
        delete _footerView;
}

void PackageSelectScene::handleButton(u32 buttons) {
    if (buttons & KEY_A) {
        Mix_PlayChannel(-1, AssetManager::enter, 0);

        switch(_focusSelection) {
            case 0:
                SceneDirector::currentScene = SCENE_PACKAGE_DOWNLOAD;
                break;

            case 1:
                break;
            
            case 2:
                break;
        }
    }

    if (buttons & KEY_B) {
        Mix_PlayChannel(-1, AssetManager::back, 0);
        SceneDirector::exitApp = true;
    }

    if (buttons & KEY_UP && _focusSelection != 0) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
        _focusSelection--;
        _manageFocus();
    }

    if (buttons & KEY_DOWN && _focusSelection != 2) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
        _focusSelection++;
        _manageFocus();
    }
}

void PackageSelectScene::render(SDL_Rect rect, double dTime) {
    Scene::render(rect, dTime);
}

void PackageSelectScene::_manageFocus() {
    _installRowView->hasFocus = false;
    _channelRowView->hasFocus = false;
    _bundleRowView->hasFocus = false;

    switch(_focusSelection) {
        case 0:
            _installRowView->hasFocus = true;
            break;
        
        case 1:
            _channelRowView->hasFocus = true;
            break;

        case 2:
            _bundleRowView->hasFocus = true;
            break;
    }
}
