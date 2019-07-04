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

#include <libconfig.h>
#include <string>
#include <vector>

namespace ku {
    class ConfigManager {
        public:
            static void initialize();
            static void dealloc();

            static std::string getHost();
            static std::vector<std::string> getFilesToIgnore();
            static bool shouldAutoUpdate();
            static bool shouldUseProxy();
            static std::string getProxy();
            static std::string getProxyUsername();
            static std::string getProxyPassword();

            static bool setFilesToIgnore(std::vector<std::string> files);

            static std::string getCurrentVersion();
            static std::vector<std::string> getInstalledFiles();
            static bool getReceivedExFATWarning();
            static bool getReceivedIgnoreConfigWarning();

            static bool setCurrentVersion(std::string version);
            static bool setInstalledFiles(std::vector<std::string> files);
            static bool setReceivedExFATWarning(bool received);
            static bool setReceivedIgnoreConfigWarning(bool received);

        private:
            static inline config_t _cfg;
            static inline config_t _internalDb;

            static bool _readBoolean(std::string key, bool def, config_t config);
            static std::string _readString(std::string key, std::string def, config_t config);
            static std::vector<std::string> _readArrayOfStrings(std::string key, std::vector<std::string> def, config_t config);

            static bool _writeBoolean(std::string key, bool value, config_t config, std::string filename);
            static bool _writeString(std::string key, std::string value, config_t config, std::string filename);
            static bool _writeArrayOfStrings(std::string key, std::vector<std::string> values, config_t config, std::string filename);
            static bool _appendArrayOfStrings(std::string key, std::vector<std::string> values, config_t config, std::string filename);

            static bool _removeSetting(std::string key, config_t config, std::string filename);

            static inline const std::string CONFIG_FILENAME = "settings.cfg";
            static inline const std::string INTERNAL_FILENAME = "internal.db";

            static inline const std::string HOST_KEY = "host";
            static inline const std::string HOST_DEF = "http://kosmos-updater.teamatlasnx.com";

            static inline const std::string IGNORE_KEY = "ignore";

            static inline const std::string AUTOUPDATE_KEY = "autoupdate";
            static inline const bool AUTOUPDATE_DEF = true;

            static inline const std::string PROXY_ENABLED_KEY = "proxy_enabled";
            static inline const bool PROXY_ENABLED_DEF = false;

            static inline const std::string PROXY_URL_KEY = "proxy_url";
            static inline const std::string PROXY_URL_DEF = "";

            static inline const std::string PROXY_USERNAME_KEY = "proxy_username";
            static inline const std::string PROXY_USERNAME_DEF = "";

            static inline const std::string PROXY_PASSWORD_KEY = "proxy_password";
            static inline const std::string PROXY_PASSWORD_DEF = "";


            static inline const std::string VERSION_KEY = "version";
            static inline const std::string VERSION_DEF = "";

            static inline const std::string INSTALLED_FILES_KEY = "installed_files";

            static inline const std::string RECEIVED_EXFAT_WARNING_KEY = "received_exfat_warning";
            static inline const bool RECEIVED_EXFAT_WARNING_DEF = false;

            static inline const std::string RECEIVED_IGNORE_CONFIG_WARNING_KEY = "received_ignore_config_warning";
            static inline const bool RECEIVED_IGNORE_CONFIG_WARNING_DEF = false;
    };
}
