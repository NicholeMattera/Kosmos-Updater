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

#include <switch.h>

#include "Theme.hpp"

namespace KosmosUpdater::Managers {
    Theme * Theme::Instance() {
        if (_instance == nullptr) {
            _instance = new Theme;
        }

        return _instance;
    }

    void Theme::updateTheme() {
        ColorSetId theme;
        setsysGetColorSetId(&theme);

        this->modalOverlay = Color(18, 27, 36, 229);
        this->focusedBorderOne = Color(0, 255, 196, 255);
        this->focusedBorderTwo = Color(22, 146, 197, 255);

        if (theme == ColorSetId_Light) {
            this->isLight = true;
            this->background = Color(235, 235, 235, 255);
            this->text = Color(45, 45, 45, 255);
            this->focusedBackground = Color(253, 253, 253, 255);
            this->focusedText = Color(50, 80, 240, 255);
        } else {
            this->isLight = false;
            this->background = Color(45, 45, 45, 255);
            this->text = Color(255, 255, 255, 255);
            this->focusedBackground = Color(31, 34, 39, 255);
            this->focusedText = Color(0, 255, 196, 255);
        }
    }
}
