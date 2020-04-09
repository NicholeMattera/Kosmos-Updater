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

#include "../Scene.hpp"
#include "../Views/TextView.hpp"

#pragma once

namespace KosmosUpdater::Scenes {
    class ExFatWarningScene : public KosmosUpdater::Scene {
        public:
            ExFatWarningScene();
            ~ExFatWarningScene();

            void buttonsDown(u32 buttons, double dTime);
            void render(SDL_Rect rect, double dTime);

        private:
            double _timeSpent = -10000;
            bool _footerVisible = false;
            bool _skipWarning = false;

            KosmosUpdater::Views::TextView * _headerTextView = NULL;
            KosmosUpdater::Views::TextView * _bodyTextView = NULL;
            KosmosUpdater::Views::TextView * _footerTextView = NULL;

            void _showAppUpdateScene();
        
    };
}
