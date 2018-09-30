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

#include "NetManager.hpp"
#include <switch.h>

void NetManager::initialize() {
    socketInitializeDefault();
    #ifdef DEBUG
        nxlinkStdio();
    #endif
    
    curl_global_init(CURL_GLOBAL_ALL);
}

void NetManager::dealloc() {
    for (u32 i = 0; i < _threads.size(); i++) {
        threadWaitForExit(&_threads.at(i));
        threadClose(&_threads.at(i));
    }

    curl_global_cleanup();
    socketExit();
}

void getLatestAppVersion() {

}

void getLatestApp() {

}

void getLatestSDFilesVersion(string channel) {

}

void getLatestSDFiles(string bundle, string channel) {

}

Result NetManager::_createThread(ThreadFunc func) {
    Thread thread;

    Result res;
    if (R_FAILED( res = threadCreate(&thread, func, nullptr, 0x2000, 0x2B, -2)))
        return res;

    if (R_FAILED( res = threadStart(&thread)))
        return res;

    _threads.push_back(thread);

    return 0;
}

void NetManager::_request(string method, string url) {

}

size_t NetManager::_writeFunction(void *ptr, size_t size, size_t nmemb, string * data) {
    return 0;
}
