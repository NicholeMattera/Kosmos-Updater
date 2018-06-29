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

#include "DownloadManager.hpp"
#include <sys/socket.h>
#include <curl/curl.h>
#include <switch.h>

using namespace std;

DownloadManager::DownloadManager(string host) {
    socketInitializeDefault();
    curl_global_init(CURL_GLOBAL_ALL);

    hostname = host;
    userAgent = string("sdfile-updater/") + VERSION;
}

DownloadManager::~DownloadManager() {
    curl_global_cleanup();
    socketExit();
}


Download * DownloadManager::getLatestAppVersion() {
    CURL *curl;
    CURLcode res;

    Download * download = new Download();
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (hostname + "/" + API_VERSION + "/app-version-number").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) download);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("! Failed to get latest app version: %s\n", curl_easy_strerror(res));
            delete download;
            download = NULL;
        }

        curl_easy_cleanup(curl);
    }

    return download;
}

Download * DownloadManager::getLatestApp() {
    CURL *curl;
    CURLcode res;

    Download * download = new Download();
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (hostname + "/" + API_VERSION + "/app-download").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) download);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("! Failed to get latest app version: %s\n", curl_easy_strerror(res));
            delete download;
            download = NULL;
        }

        curl_easy_cleanup(curl);
    }

    return download;
}

Download * DownloadManager::getLatestVersion(string channel) {
    CURL *curl;
    CURLcode res;

    Download * download = new Download();
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (hostname + "/" + API_VERSION + "/version-number/" + channel).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) download);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("! Failed to get latest version: %s\n", curl_easy_strerror(res));
            delete download;
            download = NULL;
        }

        curl_easy_cleanup(curl);
    }

    return download;
}

Download * DownloadManager::getLatestFiles(string launcher, string channel) {
    CURL *curl;
    CURLcode res;

    Download * download = new Download();
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (hostname + "/" + API_VERSION + "/download/" + launcher + "/" + channel).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) download);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("! Failed to get latest SD Files: %s\n", curl_easy_strerror(res));
            delete download;
            download = NULL;
        }

        curl_easy_cleanup(curl);
    }

    return download;
}

size_t DownloadManager::writeData(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    Download * download = (Download *) userp;

    download->data = (char *) realloc(download->data, download->size + realsize + 1);
    if (download->data == NULL) {
        printf("! Not enough memory.\n");
        return 0;
    }

    memcpy(&(download->data[download->size]), contents, realsize);
    download->size += realsize;
    download->data[download->size] = 0;

    return realsize;
}