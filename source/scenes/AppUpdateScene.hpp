// Kosmos Updater
// Copyright (C) 2019 Steven Mattera
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

#include <string>

#include "../models/NetRequest.hpp"
#include "../Scene.hpp"
#include "../views/FooterView.hpp"
#include "../views/HeaderView.hpp"
#include "../views/StatusView.hpp"
#include "../views/UpdateView.hpp"

using namespace std;

class AppUpdateScene : public ku::Scene {
    public:
        AppUpdateScene();
        ~AppUpdateScene();
        
        void handleButton(u32 buttons, double dTime);
        void render(SDL_Rect rect, double dTime);

    private:
        string _latestAppVersion;
        int _latestAppMajorVersion;
        int _latestAppMinorVersion;
        int _latestAppPatchVersion;

        ku::models::NetRequest * _versionRequest;
        ku::models::NetRequest * _appRequest;
        
        ku::views::HeaderView * _headerView;
        ku::views::UpdateView * _updateView;
        ku::views::StatusView * _statusView;
        ku::views::FooterView * _footerView;
        
        void _updateVersionRequest();
        void _updateAppRequest();
        void _showStatus(string text, string subtext);
        void _parseLatestAppVersion();
};
