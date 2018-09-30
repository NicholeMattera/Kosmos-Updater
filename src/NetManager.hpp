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

#pragma once

#include <switch.h>
#include <curl/curl.h>
#include <string>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

class NetManager {
    public:
        static void initialize();
        static void dealloc();

        static void getLatestAppVersion();
        static void getLatestApp();
        static void getLatestSDFilesVersion(string channel);
        static void getLatestSDFiles(string bundle, string channel);

    private:
        static inline vector<Thread> _threads;
        
        static Result _createThread(ThreadFunc func);
        static void _request(string method, string url);
        static size_t _writeFunction(void *ptr, size_t size, size_t nmemb, string * data);
};
