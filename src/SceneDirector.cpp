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

SDL_Window * SceneDirector::window = NULL;
SDL_Renderer * SceneDirector::renderer = NULL;
Scenes SceneDirector::currentScene = SCENE_APP_UPDATE;
bool SceneDirector::exitApp = false;

SceneDirector::SceneDirector() {
    socketInitializeDefault();
    nxlinkStdio();

    romfsInit();
    setsysInitialize();
    plInitialize();

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

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
}

SceneDirector::~SceneDirector() {
    if (SceneDirector::renderer != NULL)
        SDL_DestroyRenderer(SceneDirector::renderer);

    if (SceneDirector::window != NULL)
        SDL_DestroyWindow(SceneDirector::window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    plExit();
    setsysExit();
    romfsExit();
    socketExit();
}

bool SceneDirector::direct() {
    _last = _now;
    _now = SDL_GetPerformanceCounter();
    double dTime = (double) ((_now - _last) * 1000 / SDL_GetPerformanceFrequency());

    hidScanInput();
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (kDown & KEY_A)
        return false;

    AssetManager::setRenderColor(AssetManager::background);
    SDL_RenderClear(SceneDirector::renderer);

    switch(SceneDirector::currentScene) {
        case SCENE_APP_UPDATE:
            break;

        case SCENE_DOWNLOADING_APP:
            break;

        case SCENE_PACKAGE_SELECT:
            break;

        case SCENE_DOWNLOADING_PACKAGE:
            break;

        case SCENE_ALL_DONE:
            break;
    }

    SDL_RenderPresent(SceneDirector::renderer);

    return !SceneDirector::exitApp;
};
