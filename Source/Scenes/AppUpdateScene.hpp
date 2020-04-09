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

#include <SDL2/SDL.h>

#include "../Scene.hpp"
#include "../Views/FooterView.hpp"
#include "../Views/HeaderView.hpp"
#include "../Views/ImageView.hpp"
#include "../Views/ProgressBarView.hpp"
#include "../Views/TextView.hpp"

#pragma once

namespace KosmosUpdater::Scenes {
    typedef enum {
        InitState = 0,
        DownloadingVersionNumberState = 1,
        DownloadedVersionNumberState = 2,
        DownloadingBinaryState = 3,
        DownloadedBinaryState = 4,
        ErrorState = 5,
    } AppUpdateState;

    class AppUpdateScene : public KosmosUpdater::Scene {
        public:
            AppUpdateScene();
            ~AppUpdateScene();

            void buttonsDown(u32 buttons, double dTime);
            void render(SDL_Rect rect, double dTime);

        private:
            SDL_Texture * _downloading = NULL;

            KosmosUpdater::Views::HeaderView * _headerView = NULL;
            KosmosUpdater::Views::ImageView * _downloadImageView = NULL;
            KosmosUpdater::Views::ProgressBarView * _progressBarView = NULL;
            KosmosUpdater::Views::TextView * _statusTextView = NULL;
            KosmosUpdater::Views::FooterView * _footerView = NULL;

            AppUpdateState _state = InitState;

            void _onProgressUpdate(double progress);
    };
}
