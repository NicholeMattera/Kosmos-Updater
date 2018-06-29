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

#include "SDFilesHelper.hpp"
#include "Utils.hpp"
#include "ZipUtil.hpp"

using namespace std;

Download * checkForNewerSDFiles(ConfigManager * configManager, DownloadManager * downloadManager, bool * error, bool * downloadSDFiles) {
    string version = configManager->getCurrentVersion();
    string channel = configManager->getChannel();

    Download * latestVersion = downloadManager->getLatestVersion(channel);
    if (latestVersion == NULL) {
        *error = true;
        *downloadSDFiles = false;
        return NULL;
    }

    if (version.empty()) {
        printf("- Unknown version of SDFiles currently installed.\n");
        *downloadSDFiles = true;
        return latestVersion;
    }

    if (version.compare(string(latestVersion->data)) == 0) {
        printf("- SDFiles is already up to date!\n");
        delete latestVersion;
        *downloadSDFiles = false;
        return NULL;
    } else {
        *downloadSDFiles = true;
        return latestVersion;
    }
}

void downloadLatestSDFiles(ConfigManager * configManager, DownloadManager * downloadManager, bool * error) {
    printf("- Downloading latest SDFiles...\n");

    string channel = configManager->getChannel();
    string launcher = configManager->getLauncherPreference();

    Download * latestFiles = downloadManager->getLatestFiles(launcher, channel);
    if (latestFiles == NULL) {
        *error = true;
        return;
    }

    printf("- Latest SDFiles downloaded!\n");
    writeFile("temp.zip", latestFiles);
    delete latestFiles;

    UnZip * zip = new UnZip("temp.zip");
    zip->ExtractAll("sdmc:");
    delete zip;

    deleteFile("temp.zip");
}
