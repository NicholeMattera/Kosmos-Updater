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

ConfigManager::ConfigManager() {
    config_init(&cfg);

    if(!config_read_file(&cfg, "settings.cfg")) {
        config_setting_t * root, * setting;
        root = config_root_setting(&cfg);

        setting = config_setting_add(root, "host", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "http://sdfu.stevenmattera.com");

        setting = config_setting_add(root, "channel", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "stable");

        setting = config_setting_add(root, "version", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "");

        setting = config_setting_add(root, "launcher", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "Hekate");

        config_write_file(&cfg, "settings.cfg");
    }
}

ConfigManager::~ConfigManager() {
    config_destroy(&cfg);
}

std::string ConfigManager::getHost() {
    const char * str;

    if (!config_lookup_string(&cfg, "host", &str))
        return "http://sdfu.stevenmattera.com";

    return std::string(str);
}

std::string ConfigManager::getChannel() {
    const char * str;

    if (!config_lookup_string(&cfg, "channel", &str))
        return "stable";

    return std::string(str);
}

std::string ConfigManager::getCurrentVersion() {
    const char * str;

    if (!config_lookup_string(&cfg, "version", &str))
        return "";

    return std::string(str);
}

std::string ConfigManager::getLauncherPreference() {
    const char * str;

    if (!config_lookup_string(&cfg, "launcher", &str))
        return "Hekate";

    return std::string(str);
}

bool ConfigManager::writeCurrentVersion(std::string version) {
    config_setting_t * root, * setting;
    root = config_root_setting(&cfg);

    setting = config_setting_get_member(root, "version");
    config_setting_set_string(setting, version.c_str());

    return config_write_file(&cfg, "settings.cfg");
}