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

#include <switch.h>

#include "../models/NetRequest.hpp"
#include "../Scene.hpp"
#include "../views/FooterView.hpp"
#include "../views/HeaderView.hpp"
#include "../views/ListRowView.hpp"
#include "../views/StatusView.hpp"
#include "../views/UpdateView.hpp"

namespace ku::scenes {
    class PackageSelectScene : public ku::Scene {
        public:
            PackageSelectScene();
            ~PackageSelectScene();

            void handleButton(u32 buttons, double dTime);
            void render(SDL_Rect rect, double dTime);

        private:
            std::string _latestVersion;
            
            ku::models::NetRequest * _versionRequest;

            ku::views::HeaderView * _headerView;
            ku::views::UpdateView * _updateView;
            ku::views::StatusView * _statusView;
            ku::views::ListRowView * _installRowView;
            ku::views::FooterView * _footerView;
        
            void _updateVersionRequest();
            void _showUpdateView();
            void _showPackageSelectViews();
            void _showStatusView(std::string text, std::string subtext);
    };
}