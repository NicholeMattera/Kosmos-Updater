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

#include "../Application.hpp"
#include "../Draw.hpp"
#include "../Managers/Theme.hpp"
#include "Root.hpp"

namespace KosmosUpdater::Scenes {
    Root::Root() {
        auto theme = Managers::Theme::Instance();

        this->background = theme->background;
        this->icon = std::make_unique<Image>("romfs:/icon.png");
    }
    
    void Root::buttonsDown(u32 buttons, double dTime) {
        if (buttons != 0) {
            Application::SwitchScene(nullptr);
        }
    }

    void Root::render(Rect rect, double dTime) {
        Draw::image(0, 0, this->icon);
    }
}
