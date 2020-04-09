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

#include <vector>
#include <memory>
#include <switch.h> 

#include "Draw.hpp"
#include "View.hpp"

#pragma once

namespace KosmosUpdater {
    class Scene {
        public:
            Color background = { 0, 0, 0, 255 };
            std::vector<std::weak_ptr<View>> subviews;

            Scene(){};

            virtual void buttonsHeld(u32 buttons, double dTime){};
            virtual void buttonsDown(u32 buttons, double dTime){};
            virtual void buttonsUp(u32 buttons, double dTime){};
            
            virtual void tick(Rect rect, double dTime){};
            virtual void render(Rect rect, double dTime);

            void addSubView(std::shared_ptr<View> view);

        private:

    };
}
