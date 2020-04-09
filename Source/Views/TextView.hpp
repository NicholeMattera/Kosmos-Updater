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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "../Models/TextLine.hpp"
#include "../View.hpp"
#include "../Constants.hpp"

#pragma once

namespace KosmosUpdater::Views {
    class TextView : public KosmosUpdater::View {
        public:
            TTF_Font * font;
            int lineHeight;
            std::string text;
            SDL_Color textColor;
            KosmosUpdater::TextAlignment textAlignment = KosmosUpdater::LEFT_ALIGN;

            TextView(TTF_Font * theFont, std::string theText, SDL_Color theTextColor);
            ~TextView();

            void onRender(SDL_Rect rect, double dTime);
            int getTextHeight();
            void setFont(TTF_Font * theFont);
            void setText(std::string theText);
            void setTextColor(SDL_Color theTextColor);
            void setLineHeight(int lineHeight);
        
        private:
            std::vector<KosmosUpdater::Models::TextLine *> _textLines;
            void _reset();
        
    };
}
