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

#include "ExFatWarningScene.hpp"
#include "../SceneDirector.hpp"
#include "../AssetManager.hpp"
#include "../ConfigManager.hpp"

ExFatWarningScene::ExFatWarningScene() {
    _timeSpent = -10000;
    _footerVisible = false;

    _headerTextView = new TextView(AssetManager::header_font, "Warning - SD Card Corruption", AssetManager::text);
    _headerTextView->frame = { 0, 122, 1280, 1 };
    _headerTextView->textAlignment = CENTER_ALIGN;

    _bodyOneTextView = new TextView(AssetManager::body_font, "SDFiles Updater may cause corruption to your SD card if it is formatted as ExFAT. This", AssetManager::text);
    _bodyOneTextView->frame = { 0, 272, 1280, 1 };
    _bodyOneTextView->textAlignment = CENTER_ALIGN;

    _bodyTwoTextView = new TextView(AssetManager::body_font, "is due to an issue with Nintendo's ExFAT drivers built into the Switch. It is recommended", AssetManager::text);
    _bodyTwoTextView->frame = { 0, 322, 1280, 1 };
    _bodyTwoTextView->textAlignment = CENTER_ALIGN;

    _bodyThreeTextView = new TextView(AssetManager::body_font, "that you use an SD card formatted as FAT32, however if you wish to continue and you are", AssetManager::text);
    _bodyThreeTextView->frame = { 0, 372, 1280, 1 };
    _bodyThreeTextView->textAlignment = CENTER_ALIGN;

    _bodyFourTextView = new TextView(AssetManager::body_font, "using ExFAT you do so at your own risk. This warning will not show up again.", AssetManager::text);
    _bodyFourTextView->frame = { 0, 422, 1280, 1 };
    _bodyFourTextView->textAlignment = CENTER_ALIGN;

    _footerTextView = new TextView(AssetManager::header_font, "Press any button to contine.", AssetManager::text);
    _footerTextView->frame = { 0, 567, 1280, 1 };
    _footerTextView->textAlignment = CENTER_ALIGN;
    _footerTextView->alpha = 0;

    addSubView(_headerTextView);
    addSubView(_bodyOneTextView);
    addSubView(_bodyTwoTextView);
    addSubView(_bodyThreeTextView);
    addSubView(_bodyFourTextView);
    addSubView(_footerTextView);
}

ExFatWarningScene::~ExFatWarningScene() {
    if (_headerTextView != NULL)
        delete _headerTextView;

    if (_bodyOneTextView != NULL)
        delete _bodyOneTextView;

    if (_bodyTwoTextView != NULL)
        delete _bodyTwoTextView;

    if (_bodyThreeTextView != NULL)
        delete _bodyThreeTextView;

    if (_bodyFourTextView != NULL)
        delete _bodyFourTextView;

    if (_footerTextView != NULL)
        delete _footerTextView;
}

void ExFatWarningScene::handleButton(u32 buttons) {
    if (buttons > 0 && _footerVisible) {
        Mix_PlayChannel(-1, AssetManager::enter, 0);
        ConfigManager::setReceivedExFATWarning(true);
        SceneDirector::currentScene = (ConfigManager::shouldAutoUpdate()) ? SCENE_APP_UPDATE : SCENE_PACKAGE_SELECT;
    }
}

void ExFatWarningScene::render(SDL_Rect rect, double dTime) {
    if (_footerVisible == false) {
        _timeSpent += dTime;

        if (_timeSpent >= 0 && _timeSpent < 1000) {
            _footerTextView->alpha = _timeSpent / 1000 * 255;
        } else if (_timeSpent >= 1) {
            _footerVisible = true;
            _footerTextView->alpha = 255;
        }
    }

    Scene::render(rect, dTime);
}
