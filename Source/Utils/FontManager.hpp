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

#include <map>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>

#pragma once

namespace KosmosUpdater::Utils {
    extern const std::string StandardFont;
    extern const std::string ChineseSimplifiedFont;
    extern const std::string ExtChineseSimplifiedFont;
    extern const std::string ChineseTraditionalFont;
    extern const std::string KOFont;
    extern const std::string NintendoExtFont;
    extern const std::string TotalFont;
    
    class FontManager {
        public:
            TTF_Font * getFont(std::string name, u8 size);
            void dealloc();

            static FontManager * Instance();

        private:
            std::map<std::string, PlFontData> _fontData;
            std::map<std::string, TTF_Font *> _fontCache;
            
            static inline FontManager * _instance = NULL;

            FontManager(){};
            FontManager(FontManager const&){};

            PlFontData _getFontData(std::string name);

    };
}
