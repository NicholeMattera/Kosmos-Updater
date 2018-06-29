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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <switch.h>

#include "AppHelper.hpp"
#include "ConfigManager.hpp"
#include "DownloadManager.hpp"
#include "SDFilesHelper.hpp"

using namespace std;

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(NULL);
    ConfigManager * configManager = new ConfigManager();
    DownloadManager * downloadManager = new DownloadManager(configManager->getHost());

    printf("SDFiles Updater version %s\n\n", VERSION);

    bool appIsUpToDate = false;
    bool error = false;

    checkForNewerApp(downloadManager, &error, &appIsUpToDate);

    if (!appIsUpToDate && !error) {
        downloadLatestApp(downloadManager, &error);
    }
    else if (appIsUpToDate && !error) {
        bool downloadSDFiles = false;
        Download * latestVersion = checkForNewerSDFiles(configManager, downloadManager, &error, &downloadSDFiles);

        if (downloadSDFiles && !error) {
            downloadLatestSDFiles(configManager, downloadManager, &error);

            if (!error) {
                printf("- Installed %s SD Files!\n", latestVersion->data);
                configManager->writeCurrentVersion(string(latestVersion->data));
            }

            delete latestVersion;
        }
    }

    printf("\nPress any button to exit.\n");

    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();

    while(appletMainLoop())
    {
        hidScanInput();
        if (hidKeysDown(CONTROLLER_P1_AUTO) >= 1) {
            break;
        }
    }

    delete configManager;
    delete downloadManager;

    gfxExit();
    return 0;
}
