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

#include "../ModalView.hpp"
#include "../models/NetRequest.hpp"
#include "../models/ThreadObj.hpp"
#include "../models/Zip.hpp"
#include "../Scene.hpp"
#include "../views/AlertView.hpp"
#include "../views/FooterView.hpp"
#include "../views/HeaderView.hpp"
#include "../views/StatusView.hpp"
#include "../views/UpdateView.hpp"

class PackageDownloadScene : public ku::Scene {
    public:
        PackageDownloadScene();
        ~PackageDownloadScene();

        void handleButton(u32 buttons, double dTime);
        void render(SDL_Rect rect, double dTime);

    private:
        ku::models::ThreadObj * _packageDelete;
        ku::models::NetRequest * _packageRequest;
        ku::models::Zip * _packageExtract;
        ku::models::ThreadObj * _packageDisableGC;
        std::string _versionNumber;
        int _numberOfFiles;

        ku::views::HeaderView * _headerView;
        ku::views::UpdateView * _updateView;
        ku::views::StatusView * _statusView;
        ku::views::FooterView * _footerView;

        ku::views::AlertView * _restartAlertView;

        void _updatePackageDelete();
        void _updatePackageRequest();
        void _updatePackageExtract();
        void _updatePackageDisableGC();
        void _showStatus(std::string text, std::string subtext, bool wasSuccessful);
        void _onAlertViewDismiss(ku::ModalView * view, bool success);
        std::string _getVersionNumber(std::string version);
};
