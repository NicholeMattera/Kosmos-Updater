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

#pragma once

#include <SDL2/SDL.h>
#include "AssetManager.hpp"
#include "Scene.hpp"

#include "scenes/AllDoneScene.hpp"
#include "scenes/AppUpdateScene.hpp"
#include "scenes/DownloadingAppScene.hpp"
#include "scenes/DownloadingPackageScene.hpp"
#include "scenes/PackageSelectScene.hpp"

typedef enum {
    SCENE_APP_UPDATE,
    SCENE_DOWNLOADING_APP,
    SCENE_PACKAGE_SELECT,
    SCENE_DOWNLOADING_PACKAGE,
    SCENE_ALL_DONE
} Scenes;

using namespace std;

class SceneDirector {
    public:
        static SDL_Window * window;
        static SDL_Renderer * renderer;
        static Scenes currentScene;
        static bool exitApp;
        
        SceneDirector();
        ~SceneDirector();

        bool direct();

    private:
        Uint64 _now;
        Uint64 _last;

        Scene * _currentScene;
        AllDoneScene * _allDoneScene;
        AppUpdateScene * _appUpdateScene;
        DownloadingAppScene * _downloadingAppScene;
        DownloadingPackageScene * _downloadingPackageScene;
        PackageSelectScene * _packageSelectScene;
};