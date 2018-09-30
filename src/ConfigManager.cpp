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

config_t ConfigManager::_cfg;

void ConfigManager::initialize() {
    config_init(&_cfg);

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

        config_write_file(&_cfg, "settings.cfg");
    }
}

void ConfigManager::dealloc() {
    config_destroy(&_cfg);
}

string ConfigManager::getHost() {
    return _read("host", "http://sdfu.stevenmattera.com");
}

string ConfigManager::getChannel() {
    return _read("channel", "stable");
}

string ConfigManager::getBundle() {
    return _read("bundle", "sdfiles");
}

string ConfigManager::getCurrentVersion() {
    return _read("version", "");
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
