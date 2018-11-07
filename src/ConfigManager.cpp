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
    config_init(&_fileDb);

    if(!config_read_file(&_cfg, "settings.cfg")) {
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

        config_write_file(&_cfg, "settings.cfg");
    }

    if(!config_read_file(&_fileDb, "files.db")) {
        config_setting_t * root;
        root = config_root_setting(&_fileDb);

        config_setting_add(root, "installed_files", CONFIG_TYPE_ARRAY);

        config_write_file(&_fileDb, "files.db");
    }
}

void ConfigManager::dealloc() {
    config_destroy(&_cfg);
    config_destroy(&_fileDb);
}

string ConfigManager::getHost() {
    return _read("host", "http://sdfu.stevenmattera.com");
}

string ConfigManager::getChannel() {
    return _read("channel", "stable");
}

string ConfigManager::getBundle() {
    string bundle = _read("bundle", "sdfiles");
    if (bundle == "hekate-nogc") {
        setBundle("hekate");
        return "hekate";
    }

    return bundle;
}

string ConfigManager::getCurrentVersion() {
    return _read("version", "");
}

vector<string> ConfigManager::getFilesToIgnore() {
    vector<string> result;
    
    config_setting_t * array = config_lookup(&_cfg, "ignore");
    if (array == NULL)
        return result;

    int count = config_setting_length(array);
    for (int i = 0; i < count; i++) {
        result.push_back(string(config_setting_get_string_elem(array, i)));
    }

    return result;
}

bool ConfigManager::shouldAutoUpdate() {
    int autoupdate;

    if (!config_lookup_bool(&_cfg, "autoupdate", &autoupdate))
        return true;

    return autoupdate;
}

bool ConfigManager::shouldUseProxy() {
    int proxyEnabled;

    if (!config_lookup_bool(&_cfg, "proxy_enabled", &proxyEnabled))
        return false;

    return proxyEnabled;
}

string ConfigManager::getProxy() {
    return _read("proxy_url", "");
}

string ConfigManager::getProxyUsername() {
    return _read("proxy_username", "");
}

string ConfigManager::getProxyPassword() {
    return _read("proxy_password", "");
}

vector<string> ConfigManager::getInstalledFiles() {
    vector<string> result;
    
    config_setting_t * array = config_lookup(&_fileDb, "installed_files");
    if (array == NULL)
        return result;

    int count = config_setting_length(array);
    for (int i = 0; i < count; i++) {
        result.push_back(string(config_setting_get_string_elem(array, i)));
    }

    return result;
}

bool ConfigManager::setChannel(string channel) {
    return _write("channel", channel);
}

bool ConfigManager::setBundle(string bundle) {
    return _write("bundle", bundle);
}

bool ConfigManager::setCurrentVersion(string version) {
    return _write("version", version);
}

bool ConfigManager::setInstalledFiles(vector<string> files) {
    config_setting_t * root = config_root_setting(&_fileDb);
    config_setting_remove(root, "installed_files");

    config_setting_t * array = config_setting_add(root, "installed_files", CONFIG_TYPE_ARRAY);
    for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
        config_setting_set_string_elem(array, -1, (*it).c_str());
    }

    return config_write_file(&_fileDb, "files.db");
}

string ConfigManager::_read(string key, string def) {
    const char * str;

    if (!config_lookup_string(&_cfg, key.c_str(), &str))
        return def;

    return string(str);
}

bool ConfigManager::_write(string key, string value) {
    config_setting_t * root, * setting;
    root = config_root_setting(&_cfg);

    setting = config_setting_get_member(root, key.c_str());
    config_setting_set_string(setting, value.c_str());

    return config_write_file(&_cfg, "settings.cfg");
}
