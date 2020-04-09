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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Application.hpp"
#include "Constants.hpp"
#include "Utils/FontManager.hpp"
#include "Utils/ThemeManager.hpp"
#include "Utils/VersionService.hpp"

using namespace KosmosUpdater::Utils;

namespace KosmosUpdater {
    Application::Application() {
        currentApplication = this;

        Result rc;
        
        #ifdef DEBUG
            nxlinkStdio();
        #endif

        rc = romfsInit();
        if (R_FAILED(rc))
            return;
        _initializedServices |= ROMFS_SERVICE;

        rc = setInitialize();
        if (R_FAILED(rc))
            return;
        _initializedServices |= SET_SERVICE;

        rc = setsysInitialize();
        if (R_FAILED(rc))
            return;
        _initializedServices |= SETSYS_SERVICE;

        rc = plInitialize();
        if (R_FAILED(rc))
            return;
        _initializedServices |= PL_SERVICE;

        if (!SDL_Init(SDL_INIT_EVERYTHING))
            return;
        _initializedServices |= SDL_SERVICE;

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
            return;
        _initializedServices |= SDL_IMG_SERVICE;

        window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
        if (!window)
            return;

        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
        if (!renderer)
            return;
        SDL_SetRenderDrawBlendMode(Application::renderer, SDL_BLENDMODE_BLEND);

        if (TTF_Init() == -1)
             return;
        _initializedServices |= SDL_TTF_SERVICE;

        _nowTime = _lastTime = SDL_GetPerformanceCounter();
        _initialized = true;
    }

    Application::~Application() {
        FontManager::Instance()->dealloc();
        
        if (_currentScene != NULL)
            delete _currentScene;

        if (_initializedServices & SDL_TTF_SERVICE)
            TTF_Quit();

        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);

        if (window != NULL)
            SDL_DestroyWindow(window);

        if (_initializedServices & SDL_IMG_SERVICE)
            IMG_Quit();

        if (_initializedServices & SDL_SERVICE)
            SDL_Quit();

        if (_initializedServices & PL_SERVICE)
            plExit();

        if (_initializedServices & SETSYS_SERVICE)
            setsysExit();

        if (_initializedServices & SET_SERVICE)
            setExit();

        if (_initializedServices & ROMFS_SERVICE)
            romfsExit();
    }

    int Application::start(Scene * scene) {
        if (!_initialized || !window || !renderer)
            return -1;

        _currentScene = scene;

        // Main Game Loop
        while (appletMainLoop())
        {
            double dTime = _getDeltaTime();

            // Handle controller inputs
            hidScanInput();
            if (modal != NULL) {
                _processControllerForModal(CONTROLLER_P1_AUTO, dTime);
            } else {
                _processControllerForScene(CONTROLLER_P1_AUTO, dTime);
            }

            if (_handlePossibleSceneChange())
                break;

            _render(dTime);

            if (_handlePossibleSceneChange())
                break;

            // Present our frame.
            SDL_RenderPresent(renderer);
        }

        return 0;
    }

    void Application::render() {
        double dTime = _getDeltaTime();
        _render(dTime);

        // Present our frame.
        SDL_RenderPresent(renderer);
    }

    void Application::switchScene(Scene * scene) {
        if (_currentScene != NULL) {
            _previousScene = _currentScene;
        }

        _currentScene = scene;
    }

    double Application::_getDeltaTime() {
        _lastTime = _nowTime;
        _nowTime = SDL_GetPerformanceCounter();
        return (double) ((_nowTime - _lastTime) * 1000 / SDL_GetPerformanceFrequency());
    }

    void Application::_processControllerForModal(HidControllerID id, double dTime) {
        if (modal->enabledEvents & BUTTON_HELD_EVENT) {
            u32 kHeld = hidKeysHeld(id);
            modal->buttonsHeld(kHeld, dTime);
        }
        
        if (modal->enabledEvents & BUTTON_DOWN_EVENT) {
            u32 kDown = hidKeysDown(id);
            modal->buttonsDown(kDown, dTime);
        }
        
        if (modal->enabledEvents & BUTTON_UP_EVENT) {
            u32 kHeld = hidKeysUp(id);
            modal->buttonsUp(kHeld, dTime);
        }
    }

    void Application::_processControllerForScene(HidControllerID id, double dTime) {
        if (_currentScene != NULL && _currentScene->enabledEvents & BUTTON_HELD_EVENT) {
            u32 kHeld = hidKeysHeld(id);
            _currentScene->buttonsHeld(kHeld, dTime);
        }
        
        if (_currentScene != NULL && _currentScene->enabledEvents & BUTTON_DOWN_EVENT) {
            u32 kDown = hidKeysDown(id);
            _currentScene->buttonsDown(kDown, dTime);
        }
        
        if (_currentScene != NULL && _currentScene->enabledEvents & BUTTON_UP_EVENT) {
            u32 kHeld = hidKeysUp(id);
            _currentScene->buttonsUp(kHeld, dTime);
        }
    }

    bool Application::_handlePossibleSceneChange() {
        // We should clean up our previous scene after switching.
        if (_previousScene != NULL) {
            delete _previousScene;
            _previousScene = NULL;
        }

        // Kill the app when there is no scene.
        if (_currentScene == NULL) {
            return true;
        }

        return false;
    }

    void Application::_render(double dTime) {
        // Clear the screen.
        SDL_SetRenderDrawColor(
            renderer,
            _currentScene->background.r,
            _currentScene->background.g,
            _currentScene->background.b,
            _currentScene->background.a
        );
        SDL_RenderClear(renderer);
        
        // Call rendering routines.
        _currentScene->render({ 0, 0, 1280, 720 }, dTime);
        if (modal != NULL) {
            modal->render({ 0, 0, 1280, 720 }, dTime);
        }
    }
}
