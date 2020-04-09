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

#include "FontManager.hpp"

namespace KosmosUpdater::Utils {
    extern const std::string StandardFont = "standard";
    extern const std::string ChineseSimplifiedFont = "chinese-simplified";
    extern const std::string ExtChineseSimplifiedFont = "ext-chinese-simplified";
    extern const std::string ChineseTraditionalFont = "chinese-traditional";
    extern const std::string KOFont = "ko";
    extern const std::string NintendoExtFont = "nintendo-ext";
    extern const std::string TotalFont = "total";

    FontManager * FontManager::Instance() {
        if (_instance == NULL) {
            _instance = new FontManager;
        }

        return _instance;
    }

    TTF_Font * FontManager::getFont(std::string name, u8 size) {
        auto fontIt = _fontCache.find(name + "_" + std::to_string(size));
        if (fontIt != _fontCache.end())
            return fontIt->second;

        PlFontData fontData;
        auto fontDataIt = _fontData.find(name);
        if (fontDataIt == _fontData.end()) {
            fontData = _getFontData(name);
            _fontData.insert(std::pair<std::string, PlFontData>(name, fontData));
        } else
            fontData = fontDataIt->second;

        TTF_Font * font = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, size);
        _fontCache.insert(std::pair<std::string, TTF_Font *>(name + "_" + std::to_string(size), font));

        return font;
    }

    void FontManager::dealloc() {
        for (auto it = _fontCache.begin(); it != _fontCache.end(); ++it) {
            TTF_CloseFont(it->second);
        }
        _fontCache.clear();
        _fontData.clear();
    }

    PlFontData FontManager::_getFontData(std::string name) {
        PlFontData data;
        if (name == ChineseSimplifiedFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ChineseSimplified);
        }
        else if (name == ExtChineseSimplifiedFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ExtChineseSimplified);
        }
        else if (name == ChineseTraditionalFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ChineseTraditional);
        }
        else if (name == KOFont) {
            plGetSharedFontByType(&data, PlSharedFontType_KO);
        }
        else if (name == NintendoExtFont) {
            plGetSharedFontByType(&data, PlSharedFontType_NintendoExt);
        }
        else if (name == TotalFont) {
            plGetSharedFontByType(&data, PlSharedFontType_Total);
        }
        else {
            plGetSharedFontByType(&data, PlSharedFontType_Standard);
        }

        return data;
    }
}
