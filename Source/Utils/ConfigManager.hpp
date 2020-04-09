/*
 * Kosmos Updater
 * Copyright (c) 2020 Nichole Mattera
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <libconfig.h++>
#include <string>
#include <switch.h>
#include <vector>

#pragma once

namespace KosmosUpdater::Utils {
    class ConfigManager {
        public:
            static ConfigManager * Instance();
        
            bool shouldAutoUpdate();
            void setAutoUpdate(bool value);

            std::vector<std::string> getFilesToIgnore();
            void pushFilesToIgnore(std::string value);
            void removeFileFromIgnore(int index);

            bool shouldInstallESPatches();
            void setInstallESPatches(bool value);

            std::string getInstalledVersion();
            void setInstalledVersion(std::string value);

            bool shouldUseProxy();
            void setUseProxy(bool value);

            std::string getProxy();
            void setProxy(std::string value);

            std::string getProxyUsername();
            void setProxyUsername(std::string value);

            std::string getProxyPassword();
            void setProxyPassword(std::string value);

            bool shouldHideExFatWarning();
            void setHideExFatWarning(bool value);

            bool synchronize();
        private:
            libconfig::Config _config;
            bool _configLoaded = false;

            static inline ConfigManager * _instance = NULL;

            ConfigManager();
            ConfigManager(ConfigManager const&){};

            void _createDefaultConfig();
            bool _kosmosToolboxConfigPath();

    };
}
