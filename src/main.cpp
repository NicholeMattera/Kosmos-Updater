// SDFile Updater
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
#include "DownloadManager.hpp"
#include "ConfigManager.hpp"
#include "ZipUtil.hpp"

using namespace std;

bool writeFile(std::string filename, Download * download);
bool deleteFile(std::string filename);
bool fileExists(std::string filename);

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(NULL);
    ConfigManager * configManager = new ConfigManager();
    DownloadManager * downloadManager = new DownloadManager(configManager->getHost());

    printf("SDFile Updater version 1.0.0\n\n");

    bool downloadSDFiles = false;
    string version = configManager->getCurrentVersion();
    string channel = configManager->getChannel();

    Download * latestVersion = downloadManager->getLatestVersion(channel);
    if (version.empty()) {
        printf("- Unknown version currently installed.\n");
        downloadSDFiles = true;
    } else if (latestVersion != NULL) {
        if (version.compare(std::string(latestVersion->data)) == 0) {
            printf("- SD files are already up to date!\n");
            delete latestVersion;
        } else {
            downloadSDFiles = true;
        }
    }

    if (downloadSDFiles) {
        printf("- Downloading latest SD files...\n");

        Download * latestFile = downloadManager->getLatestFiles(channel);
        if (latestFile != NULL) {
            printf("- Latest SD files downloaded!\n");

            writeFile("latest.zip", latestFile);
            delete latestFile;

            UnZip * zip = new UnZip("latest.zip");
            std::string launcher = configManager->getLauncherPreference();

            int i = 0;
	        for(;;) {
                int code;
                if(i == 0) {
                    code = unzGoToFirstFile(zip->fileToUnzip);
                    i++;
                } else {
                    code = unzGoToNextFile(zip->fileToUnzip);
                }
		        
                if(code == UNZ_END_OF_LIST_OF_FILE)
                    break;
		
                unz_file_info_s * fileInfo = zip->GetFileInfo();
                std::string internalFileName = zip->GetFullFileName(fileInfo);

                if(fileInfo->uncompressed_size != 0 && fileInfo->compression_method != 0) {
                    if (internalFileName.compare(0, launcher.length(), launcher) == 0) {
                        std::string externalFileName = "sdmc:/" + std::string(internalFileName).substr(launcher.length() + 1, internalFileName.length() - (launcher.length() + 1));
                        zip->ExtractFile(internalFileName.c_str(), externalFileName.c_str());
                    }
                }

                free(fileInfo);
	        }

            delete zip;
            deleteFile("latest.zip");

            printf("- Installed %s SD Files!\n", latestVersion->data);
            configManager->writeCurrentVersion(std::string(latestVersion->data));
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

/* Helper Functions */

bool writeFile(std::string filename, Download * download) {
    deleteFile(filename);

    FILE * file = fopen(filename.c_str(), "w");
    if (!file) {
        return false;
    }

    size_t result = fwrite(download->data, sizeof(char), download->size, file);
    fclose(file);

    return (result == download->size);
}

bool deleteFile(std::string filename) {
    if (fileExists(filename)) {
        return remove(filename.c_str()) == 0;
    }

    return false;
}

bool fileExists(std::string filename) {
    FILE * file = fopen(filename.c_str(), "r");

    if (file) {
        fclose(file);
        return true;
    }

    return false;
}
