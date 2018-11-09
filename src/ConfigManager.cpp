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

#include "ConfigManager.hpp"

void ConfigManager::initialize() {
    config_init(&_cfg);
    config_init(&_internalDb);

    if(!config_read_file(&_cfg, CONFIG_FILENAME.c_str())) {
        config_setting_t * root, * setting;
        root = config_root_setting(&_cfg);

        setting = config_setting_add(root, "host", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "http://sdfu.stevenmattera.com");

        setting = config_setting_add(root, "channel", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "stable");

        setting = config_setting_add(root, "bundle", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "sdfiles");

        setting = config_setting_add(root, "version", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "");

        setting = config_setting_add(root, "ignore", CONFIG_TYPE_ARRAY);

        setting = config_setting_add(root, "autoupdate", CONFIG_TYPE_BOOL);
        config_setting_set_bool(setting, true);

        setting = config_setting_add(root, "proxy_enabled", CONFIG_TYPE_BOOL);
        config_setting_set_bool(setting, false);
        
        setting = config_setting_add(root, "proxy_url", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "");

        setting = config_setting_add(root, "proxy_username", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "");

        setting = config_setting_add(root, "proxy_password", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "");

        config_write_file(&_cfg, CONFIG_FILENAME.c_str());
    }

    if(!config_read_file(&_internalDb, INTERNAL_FILENAME.c_str())) {
        config_setting_t * root, * setting;
        root = config_root_setting(&_internalDb);

        setting = config_setting_add(root, "installed_files", CONFIG_TYPE_ARRAY);
        
        setting = config_setting_add(root, "disabled_game_cart", CONFIG_TYPE_BOOL);
        config_setting_set_bool(setting, false);
        
        setting = config_setting_add(root, "received_exfat_warning", CONFIG_TYPE_BOOL);
        config_setting_set_bool(setting, false);

        config_write_file(&_internalDb, INTERNAL_FILENAME.c_str());
    }
}

void ConfigManager::dealloc() {
    config_destroy(&_cfg);
    config_destroy(&_internalDb);
}

string ConfigManager::getHost() {
    return _readString("host", "http://sdfu.stevenmattera.com", _cfg);
}

string ConfigManager::getChannel() {
    return _readString("channel", "stable", _cfg);
}

string ConfigManager::getBundle() {
    string bundle = _readString("bundle", "sdfiles", _cfg);

    if (bundle == "hekate-nogc") {
        setBundle("hekate");
        return "hekate";
    }

    return bundle;
}

string ConfigManager::getCurrentVersion() {
    return _readString("version", "", _cfg);
}

vector<string> ConfigManager::getFilesToIgnore() {
    vector<string> defaultValue;
    return _readArrayOfStrings("ignore", defaultValue, _cfg);
}

bool ConfigManager::shouldAutoUpdate() {
    return _readBoolean("autoupdate", true, _cfg);
}

bool ConfigManager::shouldUseProxy() {
    return _readBoolean("proxy_enabled", false, _cfg);
}

string ConfigManager::getProxy() {
    return _readString("proxy_url", "", _cfg);
}

string ConfigManager::getProxyUsername() {
    return _readString("proxy_username", "", _cfg);
}

string ConfigManager::getProxyPassword() {
    return _readString("proxy_password", "", _cfg);
}

bool ConfigManager::setChannel(string channel) {
    return _writeString("channel", channel, _cfg, CONFIG_FILENAME);
}

bool ConfigManager::setBundle(string bundle) {
    return _writeString("bundle", bundle, _cfg, CONFIG_FILENAME);
}

bool ConfigManager::setCurrentVersion(string version) {
    return _writeString("version", version, _cfg, CONFIG_FILENAME);
}

vector<string> ConfigManager::getInstalledFiles() {
    vector<string> defaultValue;
    return _readArrayOfStrings("installed_files", defaultValue, _internalDb);
}

bool ConfigManager::getDisabledGameCart() {
    return _readBoolean("disabled_game_cart", false, _internalDb);
}

bool ConfigManager::getReceivedExFATWarning() {
    return _readBoolean("received_exfat_warning", false, _internalDb);
}

bool ConfigManager::setInstalledFiles(vector<string> files) {
    return _writeArrayOfStrings("installed_files", files, _internalDb, INTERNAL_FILENAME);
}

bool ConfigManager::setDisabledGameCart(bool disabled) {
    return _writeBoolean("disabled_game_cart", disabled, _internalDb, INTERNAL_FILENAME);
}

bool ConfigManager::setReceivedExFATWarning(bool received) {
    return _writeBoolean("received_exfat_warning", received, _internalDb, INTERNAL_FILENAME);
}

// Private Methods

bool ConfigManager::_readBoolean(string key, bool def, config_t config) {
    int result;

    if (!config_lookup_bool(&_cfg, key.c_str(), &result))
        return def;

    return result;
}

string ConfigManager::_readString(string key, string def, config_t config) {
    const char * result;

    if (!config_lookup_string(&config, key.c_str(), &result))
        return def;

    return string(result);
}

vector<string> _readArrayOfStrings(string key, vector<string> def, config_t config) {
    vector<string> result;
    
    config_setting_t * array = config_lookup(&config, key.c_str());
    if (array == NULL)
        return def;

    int count = config_setting_length(array);
    for (int i = 0; i < count; i++) {
        result.push_back(string(config_setting_get_string_elem(array, i)));
    }

    return result;
}

bool ConfigManager::_writeBoolean(string key, bool value, config_t config, string filename) {
    config_setting_t * root, * setting;
    root = config_root_setting(&_cfg);

    setting = config_setting_get_member(root, key.c_str());
    if (setting == NULL) {
        setting = config_setting_add(root, key.c_str(), CONFIG_TYPE_BOOL);
    }

    config_setting_set_bool(setting, value);

    return config_write_file(&_cfg, filename.c_str());
}

bool ConfigManager::_writeString(string key, string value, config_t config, string filename) {
    config_setting_t * root, * setting;
    root = config_root_setting(&_cfg);

    setting = config_setting_get_member(root, key.c_str());
    if (setting == NULL) {
        setting = config_setting_add(root, key.c_str(), CONFIG_TYPE_STRING);
    }

    config_setting_set_string(setting, value.c_str());

    return config_write_file(&_cfg, filename.c_str());
}

bool _writeArrayOfStrings(string key, vector<string> value, config_t config, string filename) {
    config_setting_t * root = config_root_setting(&config);
    config_setting_remove(root, key.c_str());

    config_setting_t * array = config_setting_add(root, key.c_str(), CONFIG_TYPE_ARRAY);
    for (vector<string>::iterator it = value.begin(); it != value.end(); it++) {
        config_setting_set_string_elem(array, -1, (*it).c_str());
    }

    return config_write_file(&config, filename.c_str());
}
