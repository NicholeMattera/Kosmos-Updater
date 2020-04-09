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

#include <list>
#include <switch.h>
#include <SDL2/SDL.h>

#pragma once

namespace KosmosUpdater {
    class View {
        public:
            SDL_Rect frame = { 0, 0, 0, 0 };
            SDL_Color background = { 0, 0, 0, 0 };
            u8 alpha = 255;
            bool isHidden = false;
            bool clipsToBounds = true;
            bool requiresRendering = true;

            View(){};
            virtual ~View();
            virtual void onRender(SDL_Rect rect, double dTime){};
            void render(SDL_Rect rect, double dTime);

            /* Controls */
            bool isFocusable = false;
            bool hasFocus = false;
            View * focusedSubview = NULL;

            /* Touch Controls */
            bool isTouchable = false;
            virtual void touchStarted(){};
            virtual void touchMoved(){};
            virtual void touchEnded(){};

            /* View Hierarchy */
            View * superview = NULL;
            std::list<View *> subviews;
            void addSubView(View * view);
            void removeSubView(View * view);

        private:
            SDL_Texture * _texture = NULL;
    };
}
