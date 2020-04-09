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

#include "ThemeManager.hpp"

namespace KosmosUpdater::Utils {
    ThemeManager * ThemeManager::Instance() {
        if (_instance == NULL) {
            _instance = new ThemeManager;
        }

        return _instance;
    }

    ThemeManager::ThemeManager() {
        setsysGetColorSetId(&colorSet);
        if (colorSet == ColorSetId_Light) {
            background = { 235, 235, 235, 255 };
            sidebard_background = { 240, 240, 240, 255 };
            header_footer_divider = { 45, 45, 45, 255 };
            header_bullet = { 121, 121, 121, 255 };
            list_divider = { 205, 205, 205, 255 };
            active_player_indicator = { 158, 228, 0, 255 };
            player_indicator = { 125, 125, 125, 255 };
            selected_background = { 253, 253, 253, 255 };
            selected_border_1 = { 0, 255, 196, 255 };
            selected_border_2 = { 22, 146, 197, 255 };
            modal_faded_background = { 18, 27, 36, 229 };
            modal_background = { 240, 240, 240, 255 };
            text = { 45, 45, 45, 255 };
            active_text = { 50, 80, 240, 255 };
            disabled_text = { 165, 165, 165, 255 };
        } else {
            background = { 45, 45, 45, 255 };
            sidebard_background = { 51, 51, 51, 255 };
            header_footer_divider = { 255, 255, 255, 255 };
            header_bullet = { 160, 160, 160, 255 };
            list_divider = { 77, 77, 77, 255 };
            active_player_indicator = { 158, 228, 0, 255 };
            player_indicator = { 125, 125, 125, 255 };
            selected_background = { 31, 34, 39, 255 };
            selected_border_1 = { 0, 255, 196, 255 };
            selected_border_2 = { 22, 146, 197, 255 };
            modal_faded_background = { 18, 27, 36, 229 };
            modal_background = { 70, 70, 70, 255 };
            text = { 255, 255, 255, 255 };
            active_text = { 0, 255, 196, 255 };
            disabled_text = { 125, 125, 125, 255 };
        }
    }

    std::string ThemeManager::getAssetDirectory() {
        std::string theme = (colorSet == ColorSetId_Light) ? "light" : "dark";
        return "romfs:/" + theme;
    }
}
