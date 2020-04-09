// Kosmos Updater
// Copyright (C) 2020 Nichole Mattera
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

#include "ModalView.hpp"
#include "Scene.hpp"
#include "scenes/AppUpdateScene.hpp"
#include "scenes/ExFatWarningScene.hpp"
#include "scenes/PackageDownloadScene.hpp"
#include "scenes/PackageSelectScene.hpp"

namespace ku {
    typedef enum {
        SCENE_EXFAT_WARNING,
        SCENE_APP_UPDATE,
        SCENE_PACKAGE_SELECT,
        SCENE_PACKAGE_DOWNLOAD
    } Scenes;

    class SceneDirector {
        public:
            static inline SceneDirector * currentSceneDirector = NULL;
            static inline SDL_Window * window = NULL;
            static inline SDL_Renderer * renderer = NULL;
            static inline ModalView * modal = NULL;
            static inline Scenes currentScene = SCENE_APP_UPDATE;
            static inline bool exitApp = false;
            
            SceneDirector();
            ~SceneDirector();

            bool direct();
            void render();

        private:
            Uint64 _now;
            Uint64 _last;

            Scene * _currentScene;
            scenes::ExFatWarningScene * _exFatWarningScene;
            scenes::AppUpdateScene * _appUpdateScene;
            scenes::PackageSelectScene * _packageSelectScene;
            scenes::PackageDownloadScene * _packageDownloadScene;

            double _getDeltaTime();
            void _render(double dTime);
    };
}
