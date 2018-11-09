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

        static bool setChannel(string channel);
        static bool setBundle(string bundle);
        static bool setCurrentVersion(string version);

        static vector<string> getInstalledFiles();
        static bool getDisabledGameCart();
        static bool getReceivedExFATWarning();

        static bool setInstalledFiles(vector<string> files);
        static bool setDisabledGameCart(bool disabled);
        static bool setReceivedExFATWarning(bool received);

    private:
        static inline const string CONFIG_FILENAME = "settings.cfg";
        static inline const string INTERNAL_FILENAME = "internal.db";

        static inline config_t _cfg;
        static inline config_t _internalDb;

        static bool _readBoolean(string key, bool def, config_t config);
        static string _readString(string key, string def, config_t config);
        static vector<string> _readArrayOfStrings(string key, vector<string> def, config_t config);

        static bool _writeBoolean(string key, bool value, config_t config, string filename);
        static bool _writeString(string key, string value, config_t config, string filename);
        static bool _writeArrayOfStrings(string key, vector<string> value, config_t config, string filename);
};
