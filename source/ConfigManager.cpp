// Kosmos Updater
// Copyright (C) 2020 Nichole Mattera
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

#include "ConfigManager.hpp"

using namespace std;

namespace ku {
    void ConfigManager::initialize() {
        config_init(&_cfg);
        config_init(&_internalDb);
        printf("%s %d\n", __FILE__, __LINE__);

        if(!config_read_file(&_internalDb, INTERNAL_FILENAME.c_str())) {
            config_setting_t * root, * setting;
            root = config_root_setting(&_internalDb);

            setting = config_setting_add(root, VERSION_KEY.c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(setting, VERSION_DEF.c_str());

            setting = config_setting_add(root, INSTALLED_FILES_KEY.c_str(), CONFIG_TYPE_ARRAY);
            
            setting = config_setting_add(root, RECEIVED_EXFAT_WARNING_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(setting, RECEIVED_EXFAT_WARNING_DEF);

            setting = config_setting_add(root, RECEIVED_IGNORE_CONFIG_WARNING_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(setting, RECEIVED_IGNORE_CONFIG_WARNING_DEF);

            setting = config_setting_add(root, IGNORE_CONFIG_FILES_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(setting, IGNORE_CONFIG_FILES_DEF);

            setting = config_setting_add(root, CONFIG_VERSION_KEY.c_str(), CONFIG_TYPE_INT);
            config_setting_set_int(setting, INTERNAL_CONFIG_VERSION);
            
            config_write_file(&_internalDb, INTERNAL_FILENAME.c_str());
        }

        if(!config_read_file(&_cfg, CONFIG_FILENAME.c_str())) {
            config_setting_t * root, * setting;
            root = config_root_setting(&_cfg);

            setting = config_setting_add(root, HOST_KEY.c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(setting, HOST_DEF.c_str());

            setting = config_setting_add(root, IGNORE_KEY.c_str(), CONFIG_TYPE_ARRAY);

            setting = config_setting_add(root, AUTOUPDATE_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(setting, AUTOUPDATE_DEF);

            setting = config_setting_add(root, PROXY_ENABLED_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(setting, PROXY_ENABLED_DEF);
            
            setting = config_setting_add(root, PROXY_URL_KEY.c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(setting, PROXY_URL_DEF.c_str());

            setting = config_setting_add(root, PROXY_USERNAME_KEY.c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(setting, PROXY_USERNAME_DEF.c_str());

            setting = config_setting_add(root, PROXY_PASSWORD_KEY.c_str(), CONFIG_TYPE_STRING);
            config_setting_set_string(setting, PROXY_PASSWORD_DEF.c_str());

            setting = config_setting_add(root, CONFIG_VERSION_KEY.c_str(), CONFIG_TYPE_INT);
            config_setting_set_int(setting, SETTING_CONFIG_VERSION);

            config_write_file(&_cfg, CONFIG_FILENAME.c_str());
        }

        _migrateConfigFiles();
    }

    void ConfigManager::dealloc() {
        config_destroy(&_cfg);
        config_destroy(&_internalDb);
    }

    string ConfigManager::getHost() {
        return _readString(HOST_KEY, HOST_DEF, _cfg);
    }

    vector<string> ConfigManager::getFilesToIgnore() {
        vector<string> defaultValue;
        return _readArrayOfStrings(IGNORE_KEY, defaultValue, _cfg);
    }

    bool ConfigManager::shouldAutoUpdate() {
        #ifdef DEBUG
            return false;
        #endif

        return _readBoolean(AUTOUPDATE_KEY, AUTOUPDATE_DEF, _cfg);
    }

    bool ConfigManager::shouldUseProxy() {
        return _readBoolean(PROXY_ENABLED_KEY, PROXY_ENABLED_DEF, _cfg);
    }

    string ConfigManager::getProxy() {
        return _readString(PROXY_URL_KEY, PROXY_URL_DEF, _cfg);
    }

    string ConfigManager::getProxyUsername() {
        return _readString(PROXY_USERNAME_KEY, PROXY_USERNAME_DEF, _cfg);
    }

    string ConfigManager::getProxyPassword() {
        return _readString(PROXY_PASSWORD_KEY, PROXY_PASSWORD_DEF, _cfg);
    }

    int ConfigManager::getSettingsConfigVersion() {
        return _readInt(CONFIG_VERSION_KEY, CONFIG_VERSION_DEF, _cfg);
    }


    bool ConfigManager::setFilesToIgnore(vector<string> files) {
        return _appendArrayOfStrings(IGNORE_KEY, files, _cfg, CONFIG_FILENAME);
    }


    string ConfigManager::getCurrentVersion() {
        return _readString(VERSION_KEY, VERSION_DEF, _internalDb);
    }

    vector<string> ConfigManager::getInstalledFiles() {
        vector<string> defaultValue;
        return _readArrayOfStrings(INSTALLED_FILES_KEY, defaultValue, _internalDb);
    }

    bool ConfigManager::getReceivedExFATWarning() {
        return _readBoolean(RECEIVED_EXFAT_WARNING_KEY, RECEIVED_EXFAT_WARNING_DEF, _internalDb);
    }

    bool ConfigManager::getReceivedIgnoreConfigWarning() {
        return _readBoolean(RECEIVED_IGNORE_CONFIG_WARNING_KEY, RECEIVED_IGNORE_CONFIG_WARNING_DEF, _internalDb);
    }

    bool ConfigManager::getIgnoreConfigFiles() {
        return _readBoolean(IGNORE_CONFIG_FILES_KEY, IGNORE_CONFIG_FILES_DEF, _internalDb);
    }

    int ConfigManager::getInternalConfigVersion() {
        return _readInt(CONFIG_VERSION_KEY, CONFIG_VERSION_DEF, _internalDb);
    }


    bool ConfigManager::setCurrentVersion(string version) {
        return _writeString(VERSION_KEY, version, _internalDb, INTERNAL_FILENAME);
    }

    bool ConfigManager::setInstalledFiles(vector<string> files) {
        return _writeArrayOfStrings(INSTALLED_FILES_KEY, files, _internalDb, INTERNAL_FILENAME);
    }

    bool ConfigManager::setReceivedExFATWarning(bool received) {
        return _writeBoolean(RECEIVED_EXFAT_WARNING_KEY, received, _internalDb, INTERNAL_FILENAME);
    }

    bool ConfigManager::setReceivedIgnoreConfigWarning(bool received) {
        return _writeBoolean(RECEIVED_IGNORE_CONFIG_WARNING_KEY, received, _internalDb, INTERNAL_FILENAME);
    }

    bool ConfigManager::setIgnoreConfigFiles(bool ignore) {
        return _writeBoolean(IGNORE_CONFIG_FILES_KEY, ignore, _internalDb, INTERNAL_FILENAME);
    }

    // Private Methods

    bool ConfigManager::_readBoolean(string key, bool def, config_t config) {
        int result;

        if (!config_lookup_bool(&config, key.c_str(), &result))
            return def;

        return result;
    }

    int ConfigManager::_readInt(std::string key, int def, config_t config) {
        int result;

        if (!config_lookup_int(&config, key.c_str(), &result))
            return def;

        return result;
    }

    string ConfigManager::_readString(string key, string def, config_t config) {
        const char * result;

        if (!config_lookup_string(&config, key.c_str(), &result))
            return def;

        return string(result);
    }

    vector<string> ConfigManager::_readArrayOfStrings(string key, vector<string> def, config_t config) {
        vector<string> result;
        
        config_setting_t * array = config_lookup(&config, key.c_str());
        if (array == NULL)
            return def;

        int count = config_setting_length(array);
        for (int i = 0; i < count; i++) {
            auto file = string(config_setting_get_string_elem(array, i));
            
            if (file.substr(0,1) == "/") {
                result.push_back("sdmc:" + file);
            }
            else if (file.substr(0,7) == "sdmc://") {
                result.push_back("sdmc:/" + file.substr(7, file.length() - 7));
            }
            else {
                result.push_back(file);
            }
        }

        return result;
    }

    bool ConfigManager::_writeBoolean(string key, bool value, config_t config, string filename) {
        config_setting_t * root, * setting;
        root = config_root_setting(&config);

        setting = config_setting_get_member(root, key.c_str());
        if (setting == NULL) {
            setting = config_setting_add(root, key.c_str(), CONFIG_TYPE_BOOL);
        }

        config_setting_set_bool(setting, value);

        return config_write_file(&config, filename.c_str());
    }

    bool ConfigManager::_writeString(string key, string value, config_t config, string filename) {
        config_setting_t * root, * setting;
        root = config_root_setting(&config);

        setting = config_setting_get_member(root, key.c_str());
        if (setting == NULL) {
            setting = config_setting_add(root, key.c_str(), CONFIG_TYPE_STRING);
        }

        config_setting_set_string(setting, value.c_str());

        return config_write_file(&config, filename.c_str());
    }

    bool ConfigManager::_writeArrayOfStrings(string key, vector<string> values, config_t config, string filename) {
        config_setting_t * root = config_root_setting(&config);
        config_setting_remove(root, key.c_str());

        config_setting_t * array = config_setting_add(root, key.c_str(), CONFIG_TYPE_ARRAY);
        for (auto const& value : values) {
            config_setting_set_string_elem(array, -1, value.c_str());
        }

        return config_write_file(&config, filename.c_str());
    }

    bool ConfigManager::_appendArrayOfStrings(string key, vector<string> values, config_t config, string filename) {
        config_setting_t * root = config_root_setting(&config);

        config_setting_t * array = config_lookup(&config, key.c_str());
        if (array == NULL)
            array = config_setting_add(root, key.c_str(), CONFIG_TYPE_ARRAY);

        for (auto const& value : values) {
            config_setting_set_string_elem(array, -1, value.c_str());
        }

        return config_write_file(&config, filename.c_str());
    }

    bool ConfigManager::_removeSetting(string key, config_t config, string filename) {
        config_setting_t * root = config_root_setting(&config);
        config_setting_remove(root, key.c_str());
        return config_write_file(&config, filename.c_str());
    }

    void ConfigManager::_migrateConfigFiles() {
        auto settingsVersion = getSettingsConfigVersion();
        auto internalVersion = getInternalConfigVersion();

        if (internalVersion < INTERNAL_CONFIG_VERSION) {
            _migrateInternalConfigFiles(internalVersion);
        }

        if (settingsVersion < SETTING_CONFIG_VERSION) {
            _migrateSettingsConfigFiles(settingsVersion);
        }
    }

    void ConfigManager::_migrateSettingsConfigFiles(int currentVersion) {
        bool configChanged = false;
        config_setting_t * root = config_root_setting(&_cfg);

        if (currentVersion < 1) {
            configChanged = true;
            // Migrate from HTTP to HTTPS.
            if (getHost() == "http://kosmos-updater.teamatlasnx.com") {
                config_setting_t * host = config_setting_get_member(root, HOST_KEY.c_str());
                config_setting_set_string(host, "https://kosmos-updater.teamatlasnx.com");
            }

            // Migrate sys-ftp path.
            config_setting_t * ignoredFiles = config_lookup(&_cfg, IGNORE_KEY.c_str());
            if (ignoredFiles != NULL) {
                int count = config_setting_length(ignoredFiles);
                for (int i = 0; i < count; i++) {
                    auto file = string(config_setting_get_string_elem(ignoredFiles, i));
                    if (file == "sdmc:/config/ftpd/config.ini") {
                        config_setting_set_string_elem(ignoredFiles, i, "sdmc:/config/sys-ftpd/config.ini");
                        setIgnoreConfigFiles(true);
                        break;
                    }
                }
            }

            // Added config version for future migrations
            config_setting_t * configVersion = config_setting_add(root, CONFIG_VERSION_KEY.c_str(), CONFIG_TYPE_INT);
            config_setting_set_int(configVersion, 1);
        }

        if (configChanged) {
            config_write_file(&_cfg, CONFIG_FILENAME.c_str());
        }
    }

    void ConfigManager::_migrateInternalConfigFiles(int currentVersion) {
        bool configChanged = false;
        config_setting_t * root = config_root_setting(&_internalDb);

        if (currentVersion < 1 && config_lookup(&_internalDb, IGNORE_CONFIG_FILES_KEY.c_str()) == NULL) {
            configChanged = true;

            // Added setting in internal to keep track of if the user is ignoring config files.
            config_setting_t * ignoreConfigFiles = config_setting_add(root, IGNORE_CONFIG_FILES_KEY.c_str(), CONFIG_TYPE_BOOL);
            config_setting_set_bool(ignoreConfigFiles, IGNORE_CONFIG_FILES_DEF);
        }

        if (currentVersion < 2) {
            configChanged = true;

            if (config_lookup(&_internalDb, CONFIG_VERSION_KEY.c_str()) == NULL) {
                // Added separate config version to internal db.
                config_setting_t * configVersion = config_setting_add(root, CONFIG_VERSION_KEY.c_str(), CONFIG_TYPE_INT);
                config_setting_set_int(configVersion, 2);
            } else {
                // How???
                config_setting_t * configVersion = config_lookup(&_internalDb, CONFIG_VERSION_KEY.c_str());
                config_setting_set_int(configVersion, 2);
            }
        }

        if (configChanged) {
            config_write_file(&_internalDb, INTERNAL_FILENAME.c_str());
        }
    }
}