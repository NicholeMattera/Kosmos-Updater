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
#include <SDL2/SDL.h>

#include "Modal.hpp"
#include "Scene.hpp"

#pragma once

namespace KosmosUpdater {
    class Application {
        public:
            static inline Application * currentApplication = NULL;
            static inline SDL_Window * window = NULL;
            static inline SDL_Renderer * renderer = NULL;
            static inline Modal * modal = NULL;

            Application();
            ~Application();

            int start(Scene * scene);
            void render();

            static void switchScene(Scene * scene);
        
        private:
            bool _initialized = false;
            u8 _initializedServices = 0;
            u64 _nowTime;
            u64 _lastTime;

            static inline Scene * _currentScene = NULL;
            static inline Scene * _previousScene = NULL;
        
            double _getDeltaTime();
            void _processControllerForModal(HidControllerID id, double dTime);
            void _processControllerForScene(HidControllerID id, double dTime);
            bool _handlePossibleSceneChange();
            void _render(double dTime);
    };
}
