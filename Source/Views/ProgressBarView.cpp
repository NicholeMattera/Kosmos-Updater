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

#include <SDL2/SDL2_gfxPrimitives.h>

#include "ProgressBarView.hpp"
#include "../Application.hpp"
#include "../Utils/ThemeManager.hpp"

using namespace KosmosUpdater;
using namespace KosmosUpdater::Utils;

namespace KosmosUpdater::Views {
    void ProgressBarView::onRender(SDL_Rect rect, double dTime) {
        SDL_Color background = ThemeManager::Instance()->disabled_text;
        SDL_Color foreground = ThemeManager::Instance()->active_text;

        // Draw background.
        roundedBoxRGBA(
            Application::renderer,
            rect.x,
            rect.y,
            rect.x + rect.w,
            rect.y + rect.h, 
            rect.h / 2,
            background.r, background.g, background.b, background.a);

        // Draw progress bar.
        int progressWidth = (rect.w - rect.h) * _progress;
        
        if (progressWidth > rect.w - rect.h)
            progressWidth = rect.w - rect.h;

        if (progressWidth < 0)
            progressWidth = 0;

        roundedBoxRGBA(
            Application::renderer,
            rect.x,
            rect.y,
            rect.x + rect.h + progressWidth,
            rect.y + rect.h, 
            rect.h / 2,
            foreground.r, foreground.g, foreground.b, foreground.a);
    }

    void ProgressBarView::setProgress(double progress) {
        _progress = progress;
        requiresRendering = true;
    }
}