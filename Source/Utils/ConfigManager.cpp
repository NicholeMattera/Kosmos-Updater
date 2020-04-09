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

#include <fstream>
#include <iostream>

#include "ConfigManager.hpp"

namespace KosmosUpdater::Utils {
    ConfigManager * ConfigManager::Instance() {
        if (_instance == NULL) {
            _instance = new ConfigManager;
        }

        return _instance;
    }

    bool ConfigManager::shouldAutoUpdate() {
        if (!_configLoaded || !_config.exists("auto_update")) return false;
        return _config.lookup("auto_update");
    }

    void ConfigManager::setAutoUpdate(bool value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("auto_update");
        setting = value;
    }

    std::vector<std::string> ConfigManager::getFilesToIgnore() {
        std::vector<std::string> value;
        if (!_configLoaded || !_config.exists("ignored_files")) return value;

        libconfig::Setting & setting = _config.lookup("ignored_files");
        for (int i = 0; i < setting.getLength(); i++) {
            value.push_back(setting[i]);
        }

        return value;
    }

    void ConfigManager::pushFilesToIgnore(std::string value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("ignored_files");
        setting.add(libconfig::Setting::TypeString);
        setting[setting.getLength() - 1] = value;
    }

    void ConfigManager::removeFileFromIgnore(int index) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("ignored_files");
        if (index >= setting.getLength() || index < 0) return;
        setting.remove(index);
    }

    bool ConfigManager::shouldInstallESPatches() {
        if (!_configLoaded || !_config.exists("install_es_patches")) return false;
        return _config.lookup("install_es_patches");
    }

    void ConfigManager::setInstallESPatches(bool value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("install_es_patches");
        setting = value;
    }

    std::string ConfigManager::getInstalledVersion() {
        if (!_configLoaded || !_config.exists("installed_version")) return "";
        return _config.lookup("installed_version");
    }

    void ConfigManager::setInstalledVersion(std::string value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("installed_version");
        setting = value;
    }

    bool ConfigManager::shouldUseProxy() {
        if (!_configLoaded) return false;
        return _config.lookup("proxy_enabled");
    }

    void ConfigManager::setUseProxy(bool value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("proxy_enabled");
        setting = value;
    }

    std::string ConfigManager::getProxy() {
        if (!_configLoaded) return "";
        return _config.lookup("proxy_url");
    }

    void ConfigManager::setProxy(std::string value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("proxy_url");
        setting = value;
    }

    std::string ConfigManager::getProxyUsername() {
        if (!_configLoaded) return "";
        return _config.lookup("proxy_username");
    }

    void ConfigManager::setProxyUsername(std::string value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("proxy_username");
        setting = value;
    }

    std::string ConfigManager::getProxyPassword() {
        if (!_configLoaded) return "";
        return _config.lookup("proxy_password");
    }

    void ConfigManager::setProxyPassword(std::string value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("proxy_password");
        setting = value;
    }

    bool ConfigManager::shouldHideExFatWarning() {
        if (!_configLoaded || !_config.exists("hide_exfat_warning")) return false;
        return _config.lookup("hide_exfat_warning");
    }

    void ConfigManager::setHideExFatWarning(bool value) {
        if (!_configLoaded) return;
        libconfig::Setting & setting = _config.lookup("hide_exfat_warning");
        setting = value;
    }

    bool ConfigManager::synchronize() {
        try {
            _config.writeFile("config.cfg");
            return true;
        } catch (...) {
            return false;
        }
    }

    ConfigManager::ConfigManager() {
        try {
            _config.readFile("config.cfg");
            _configLoaded = true;
        } catch (...) {
            _createDefaultConfig();
        }
    }

    void ConfigManager::_createDefaultConfig() {
        try {
            _config.readFile("romfs:/config.cfg");
            _configLoaded = true;

            if (_kosmosToolboxConfigPath())
                pushFilesToIgnore("sdmc:/switch/KosmosToolbox/config.json");

            synchronize();
        } catch (...) {
            _configLoaded = false;
        }
    }

    bool ConfigManager::_kosmosToolboxConfigPath() {
        std::ifstream file("sdmc:/switch/KosmosToolbox/config.json");
        if (file.fail())
            return false;

        file.close();
        return true;
    }
}
