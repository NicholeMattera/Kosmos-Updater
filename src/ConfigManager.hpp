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

#include <libconfig.h>
#include <string>
#include <vector>

using namespace std;

class ConfigManager {
    public:
        static void initialize();
        static void dealloc();

        static string getHost();
        static string getChannel();
        static string getBundle();
        static string getCurrentVersion();
        static vector<string> getFilesToIgnore();
        static bool shouldAutoUpdate();
        static bool shouldUseProxy();
        static string getProxy();
        static string getProxyUsername();
        static string getProxyPassword();

        static vector<string> getInstalledFiles();

        static bool setChannel(string channel);
        static bool setBundle(string bundle);
        static bool setCurrentVersion(string version);
        static bool setInstalledFiles(vector<string> files);

    private:
        static inline config_t _cfg;
        static inline config_t _fileDb;
        static string _read(string key, string def);
        static bool _write(string key, string value);
};
