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

#include "Modal.hpp"
#include "Utils/ThemeManager.hpp"
#include "Application.hpp"

using namespace KosmosUpdater::Utils;

namespace KosmosUpdater {
    void Modal::buttonsHeld(u32 buttons, double dTime) {}
    void Modal::buttonsDown(u32 buttons, double dTime) {}
    void Modal::buttonsUp(u32 buttons, double dTime) {}

    void Modal::render(SDL_Rect rect, double dTime) {
        // Draw Faded background.
        SDL_Color background = ThemeManager::Instance()->modal_faded_background;
        SDL_SetRenderDrawColor(Application::renderer, background.r, background.g, background.b, background.a);
        SDL_RenderFillRect(Application::renderer, &rect);

        View::render(rect, dTime);
    }

    void Modal::show() {
        Application::modal = this;
    }

    void Modal:: dismiss(bool success) {
        Application::modal = NULL;
        if (onDismiss) {
            onDismiss(this, success);
        }
    }
}
