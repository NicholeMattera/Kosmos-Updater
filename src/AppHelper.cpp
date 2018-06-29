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

#include "AppHelper.hpp"
#include "ConfigManager.hpp"
#include "Utils.hpp"

using namespace std;

void checkForNewerApp(DownloadManager * downloadManager, bool * error, bool * appIsUpToDate) {
    Download * latestAppVersion = downloadManager->getLatestAppVersion();
    if (latestAppVersion == NULL) {
        *error = true;
        return;
    }

    if (string(VERSION).compare(latestAppVersion->data) == 0) {
        printf("- SDFiles Updater is up to date!\n");
        *appIsUpToDate = true;
    } else {
        *appIsUpToDate = false;
    }
}

void downloadLatestApp(DownloadManager * downloadManager, bool * error) {
    printf("- Downloading latest SDFiles Updater...\n");

    Download * latestApp = downloadManager->getLatestApp();
    if (latestApp == NULL) {
        *error = true;
        return;
    }

    if (writeFile("SDFilesUpdater.nro", latestApp)) {
        printf("- SDFiles Updater has been updated, please restart the app to check for SDFiles Updates.\n");
    }
}
