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

#include "HeaderView.hpp"
#include "../Application.hpp"
#include "../Utils/ThemeManager.hpp"
#include "../Utils/FontManager.hpp"

using namespace KosmosUpdater;
using namespace KosmosUpdater::Utils;

namespace KosmosUpdater::Views {
    HeaderView::HeaderView(std::string title, bool showIcon) {
        this->background = ThemeManager::Instance()->background;

        _showIcon = showIcon;

        if (showIcon) {
            std::string path = ThemeManager::Instance()->getAssetDirectory() + "/icon.png";
            SDL_Surface * image = IMG_Load(path.c_str());
            _icon = SDL_CreateTextureFromSurface(Application::renderer, image);
            SDL_FreeSurface(image);

            _iconImageView = new ImageView(_icon);
            _iconImageView->frame = { 74, 29, 30, 44 };
            addSubView(_iconImageView);
        }

        SDL_Color textColor = ThemeManager::Instance()->text;
        TTF_Font * headerFont = FontManager::Instance()->getFont(StandardFont, 28);
        _titleTextView = new TextView(headerFont, title, textColor);
        _titleTextView->frame = {
            (showIcon) ? 132 : 74,
            36,
            (showIcon) ? 1074 : 1132,
            33
        };
        addSubView(_titleTextView);
    }

    HeaderView::~HeaderView() {
        if (_titleTextView != NULL)
            delete _titleTextView;

        if (_icon != NULL)
            SDL_DestroyTexture(_icon);
            
        if (_iconImageView != NULL)
            delete _iconImageView;
    }

    void HeaderView::onRender(SDL_Rect rect, double dTime) {
        // Divider
        SDL_Color divider = ThemeManager::Instance()->header_footer_divider;
        SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
        SDL_RenderDrawLine(Application::renderer, 30, rect.y + rect.h - 1, rect.x + rect.w - 30, rect.y + rect.h - 1);
    }
}