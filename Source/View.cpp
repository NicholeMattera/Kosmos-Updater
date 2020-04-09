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

#include "View.hpp"
#include "Application.hpp"

namespace KosmosUpdater {
    View::~View() {
        if (_texture != NULL) {
            SDL_DestroyTexture(_texture);
        }

        subviews.clear();
    }

    void View::render(SDL_Rect rect, double dTime) {
        if (background.a != 0) {
            SDL_SetRenderDrawColor(Application::renderer, background.r, background.g, background.b, background.a);
            SDL_RenderFillRect(Application::renderer, &rect);
        }

        for (auto const& view : subviews) {
            if (!view->isHidden) {
                SDL_Rect subviewFrame = view->frame;
                view->render({ rect.x + subviewFrame.x, rect.y + subviewFrame.y, subviewFrame.w, subviewFrame.h }, dTime);
            }
        }

        SDL_Rect bounds = { 0, 0, 0, 0 };
        if (clipsToBounds) {
            bounds = rect;
        } else {
            SDL_GetWindowSize(Application::window, &bounds.w, &bounds.h);
        }

        if (requiresRendering) {
            requiresRendering = false;

            if (_texture == NULL) {
                _texture = SDL_CreateTexture(Application::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bounds.w, bounds.h);
                SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
            }

            SDL_SetRenderTarget(Application::renderer, _texture);
            SDL_SetRenderDrawColor(Application::renderer, 0, 0, 0, 0);
            SDL_RenderClear(Application::renderer);

            if (clipsToBounds) {
                onRender({ 0, 0, rect.w, rect.h }, dTime);
            } else {
                onRender(rect, dTime);
            }

            SDL_SetRenderTarget(Application::renderer, NULL);
        }

        SDL_SetTextureAlphaMod(_texture, alpha);
        SDL_RenderCopy(Application::renderer, _texture, NULL, &bounds);
    }

    void View::addSubView(View * view) {
        view->superview = view;
        subviews.push_back(view);
    }

    void View::removeSubView(View * view) {
        view->superview = NULL;
        subviews.remove(view);
    }
}
