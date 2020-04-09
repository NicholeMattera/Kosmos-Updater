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

#include "AppUpdateScene.hpp"
#include "ExFatWarningScene.hpp"
#include "../Application.hpp"
#include "../Utils/ConfigManager.hpp"
#include "../Utils/FontManager.hpp"
#include "../Utils/StringManager.hpp"
#include "../Utils/ThemeManager.hpp"
#include "../Views/TextView.hpp"

using namespace KosmosUpdater;
using namespace KosmosUpdater::Utils;
using namespace KosmosUpdater::Views;

namespace KosmosUpdater::Scenes {
    ExFatWarningScene::ExFatWarningScene() {
        auto cm = ConfigManager::Instance();
        auto fm = FontManager::Instance();
        auto tm = ThemeManager::Instance();
        auto sm = StringManager::Instance();

        if (cm->shouldHideExFatWarning()) {
            _skipWarning = true;
            return;
        }

        background = tm->background;

        auto textColor = tm->text;
        auto headerFont = fm->getFont(StandardFont, 28);
        auto bodyFont = fm->getFont(StandardFont, 23);

        _headerTextView = new TextView(headerFont, sm->getLocalizedString(WarningHeader), textColor);
        _headerTextView->frame = { 0, 122, 1280, 33 };
        _headerTextView->textAlignment = CENTER_ALIGN;

        _bodyTextView = new TextView(bodyFont, sm->getLocalizedString(WarningBody), textColor);
        _bodyTextView->frame = { 0, 272, 1280, 178 };
        _bodyTextView->textAlignment = CENTER_ALIGN;
        _bodyTextView->lineHeight = 50;

        _footerTextView = new TextView(headerFont, sm->getLocalizedString(WarningFooter), textColor);
        _footerTextView->frame = { 0, 567, 1280, 34 };
        _footerTextView->textAlignment = CENTER_ALIGN;
        _footerTextView->alpha = 0;

        addSubView(_headerTextView);
        addSubView(_bodyTextView);
        addSubView(_footerTextView);
    }

    ExFatWarningScene::~ExFatWarningScene() {
        if (_headerTextView != NULL)
            delete _headerTextView;

        if (_bodyTextView != NULL)
            delete _bodyTextView;

        if (_footerTextView != NULL)
            delete _footerTextView;
    }

    void ExFatWarningScene::buttonsDown(u32 buttons, double dTime) {
        if (buttons > 0 && _footerVisible) {
            ConfigManager::Instance()->setHideExFatWarning(true);
            ConfigManager::Instance()->synchronize();

            _showAppUpdateScene();
        }
    }

    void ExFatWarningScene::render(SDL_Rect rect, double dTime) {
        if (_skipWarning) {
            _showAppUpdateScene();
        } else if (_footerVisible == false) {
            _timeSpent += dTime;

            if (_timeSpent >= 0 && _timeSpent < 1000) {
                _footerTextView->alpha = _timeSpent / 1000 * 255;
            } else if (_timeSpent >= 1 && !_footerVisible) {
                _footerVisible = true;
                _footerTextView->alpha = 255;
            }
        }
        
        Scene::render(rect, dTime);
    }

    void ExFatWarningScene::_showAppUpdateScene() {
        Application::switchScene(new AppUpdateScene());
    }
}
