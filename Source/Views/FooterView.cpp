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

#include "FooterView.hpp"
#include "../Application.hpp"
#include "../Utils/ThemeManager.hpp"

using namespace KosmosUpdater;
using namespace KosmosUpdater::Utils;
using namespace KosmosUpdater::Models;

namespace KosmosUpdater::Views {
    FooterView::FooterView() {
        this->background = ThemeManager::Instance()->background;

    }

    FooterView::~FooterView() {

    }

    void FooterView::onRender(SDL_Rect rect, double dTime) {
        // Divider
        SDL_Color divider = ThemeManager::Instance()->header_footer_divider;
        SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
        SDL_RenderDrawLine(Application::renderer, rect.x + 30, rect.y, rect.w - 30, rect.y);

        int x = rect.x + rect.w - 60;
        for (auto const & action : actions) {
            if (x < 0) {
                _renderButton(action);
                x -= 77;
            }
        }
    }

    void FooterView::_renderButton(FooterAction * action) {

    }
}