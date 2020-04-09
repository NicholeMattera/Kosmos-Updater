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

#include <memory>
#include <switch.h>

#include "Scene.hpp"

#pragma once

namespace KosmosUpdater {
    class Application {
        public:
            static inline u8 * CurrentFramebuffer = nullptr;
            static inline u32 Stride = 0;

            Application();
            ~Application();

            int start(std::shared_ptr<Scene> scene);
            void render();

            static void SwitchScene(std::shared_ptr<Scene> scene);

        private:
            NWindow * _window;
            Framebuffer _framebuffer;
            u64 _nowTime;
            u64 _lastTime;

            static inline std::shared_ptr<Scene> _CurrentScene = nullptr;

            double _getDeltaTime();
            void _processControllerForScene(HidControllerID id, double dTime);
            void _render(double dTime);
    };
}
