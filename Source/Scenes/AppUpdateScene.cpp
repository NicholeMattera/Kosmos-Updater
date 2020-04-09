/*
 * Kosmos Updater
 * Copyright (c) 2020 Nichole Mattera
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <SDL2/SDL_image.h>

#include "AppUpdateScene.hpp"
#include "../Application.hpp"
#include "../Utils/FontManager.hpp"
#include "../Utils/StringManager.hpp"
#include "../Utils/ThemeManager.hpp"
#include "../Utils/VersionService.hpp"

using namespace KosmosUpdater;
using namespace KosmosUpdater::Utils;
using namespace KosmosUpdater::Views;

namespace KosmosUpdater::Scenes {
    AppUpdateScene::AppUpdateScene() {
        auto fm = FontManager::Instance();
        auto sm = StringManager::Instance();
        auto tm = ThemeManager::Instance();

        background = tm->background;

        _headerView = new HeaderView(sm->getLocalizedString(AppTitle), true);
        _headerView->frame = { 0, 0, 1280, 88 };
        addSubView(_headerView);

        auto path = tm->getAssetDirectory() + "/downloading.png";
        auto image = IMG_Load(path.c_str());
        _downloading = SDL_CreateTextureFromSurface(Application::renderer, image);
        SDL_FreeSurface(image);

        _downloadImageView = new ImageView(_downloading);
        _downloadImageView->frame = { 400, 200, 479, 197 };
        addSubView(_downloadImageView);

        _progressBarView = new ProgressBarView();
        _progressBarView->frame = { 437, 457, 411, 10 };
        addSubView(_progressBarView);

        auto textColor = tm->text;
        auto subBodyFont = fm->getFont(StandardFont, 18);
        _statusTextView = new TextView(subBodyFont, sm->getLocalizedString(CheckingKosmosUpdaterUpdate), textColor);
        _statusTextView->frame = { 0, 491, 1280, 21 };
        _statusTextView->textAlignment = CENTER_ALIGN;
        addSubView(_statusTextView);

        _footerView = new FooterView();
        _footerView->frame = { 0, 647, 1280, 73 };
        addSubView(_footerView);
    }

    AppUpdateScene::~AppUpdateScene() {
        if (_headerView != NULL)
            delete _headerView;

        if (_downloading != NULL)
            SDL_DestroyTexture(_downloading);

        if (_downloadImageView != NULL)
            delete _downloadImageView;

        if (_progressBarView != NULL)
            delete _progressBarView;

        if (_statusTextView != NULL)
            delete _statusTextView;

        if (_footerView != NULL)
            delete _footerView;
    }

    void AppUpdateScene::buttonsDown(u32 buttons, double dTime) {
        if (_state == ErrorState) {
            if (buttons & KEY_A) {
                Application::switchScene(NULL);
            }
        } else {
            if (buttons & KEY_A) {
                Application::switchScene(NULL);
            }
        }
    }

    void AppUpdateScene::render(SDL_Rect rect, double dTime) {
        Scene::render(rect, dTime);
        
        if (_state == InitState) {
            _state = DownloadingVersionNumberState;
            // TODO: Download Version Number.
        } else if (_state == DownloadedVersionNumberState) {
            // TODO: Check if version number is newer, then switch scense or download binary.
        } else if (_state == DownloadedBinaryState) {
            // TODO: Unload romfs, replace binary, and switch scenes.
        }
    }

    void AppUpdateScene::_onProgressUpdate(double progress) {
        _progressBarView->setProgress(progress);
        Application::currentApplication->render();
    }
}
