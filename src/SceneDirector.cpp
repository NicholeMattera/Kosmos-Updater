// SDFiles Updater
// Copyright (C) 2018 Steven Mattera
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <switch.h>
#include "SceneDirector.hpp"

SceneDirector::SceneDirector() {
    romfsInit();
    setsysInitialize();
    plInitialize();

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        return;

    SceneDirector::window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
    if (!SceneDirector::window)
        return;

    SceneDirector::renderer = SDL_CreateRenderer(SceneDirector::window, 0, SDL_RENDERER_SOFTWARE);
    if (!SceneDirector::renderer)
        return;

    TTF_Init();

    _now = SDL_GetPerformanceCounter();
    _last = 0;
    _appUpdateScene = NULL;
    _packageSelectScene = NULL;
    _packageDownloadScene = NULL;
    _currentScene = NULL;
}

SceneDirector::~SceneDirector() {
    if (_appUpdateScene != NULL)
        delete _appUpdateScene;

    if (_packageSelectScene != NULL)
        delete _packageSelectScene;

    if (_packageDownloadScene != NULL)
        delete _packageDownloadScene;

    TTF_Quit();

    if (SceneDirector::renderer != NULL)
        SDL_DestroyRenderer(SceneDirector::renderer);

    if (SceneDirector::window != NULL)
        SDL_DestroyWindow(SceneDirector::window);

    IMG_Quit();
    SDL_Quit();

    plExit();
    setsysExit();
    romfsExit();
}

bool SceneDirector::direct() {
    _last = _now;
    _now = SDL_GetPerformanceCounter();
    double dTime = (double) ((_now - _last) * 1000 / SDL_GetPerformanceFrequency());

    hidScanInput();
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    // TODO: Handle touch controls

    AssetManager::setRenderColor(AssetManager::background);
    SDL_RenderClear(SceneDirector::renderer);

    // Unload previous scenes
    switch(SceneDirector::currentScene) {
        case SCENE_PACKAGE_SELECT:
            if (_appUpdateScene != NULL) {
                delete _appUpdateScene;
                _appUpdateScene = NULL;
            }
            break;

        case SCENE_PACKAGE_DOWNLOAD:
            if (_appUpdateScene != NULL) {
                delete _appUpdateScene;
                _appUpdateScene = NULL;
            }

            if (_packageSelectScene != NULL) {
                delete _packageSelectScene;
                _packageSelectScene = NULL;
            }
            break;

        default:
            /* Do Nothing */
            break;
    }

    // Load new scenes
    switch(SceneDirector::currentScene) {
        case SCENE_APP_UPDATE:
            if (_appUpdateScene == NULL)
                _appUpdateScene = new AppUpdateScene();
            
            _currentScene = _appUpdateScene;
            break;

        case SCENE_PACKAGE_SELECT:
            if (_packageSelectScene == NULL)
                _packageSelectScene = new PackageSelectScene();
            
            _currentScene = _packageSelectScene;
            break;

        case SCENE_PACKAGE_DOWNLOAD:
            if (_packageDownloadScene == NULL)
                _packageDownloadScene = new PackageDownloadScene();
            
            _currentScene = _packageDownloadScene;
            break;
    }

    _currentScene->handleButton(kDown);
    _currentScene->render({ 0, 0, 1280, 720 }, dTime);

    SDL_RenderPresent(SceneDirector::renderer);

    return !SceneDirector::exitApp;
}
