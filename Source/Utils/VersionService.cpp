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

#include <switch.h>

#include "VersionService.hpp"

namespace KosmosUpdater::Utils {
    std::string VersionService::getCurrentVersion() {
        return std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH);
    }

    bool VersionService::isNewerVersion(std::string currentVersion, std::string remoteVersion) {
        u8 currentVersionNumber = 0;
        size_t currentPos = currentVersion.find('.');
        if (currentPos != std::string::npos) {
            std::string currentSegment = currentVersion.substr(0, currentPos);
            currentVersionNumber = stoi(currentSegment);
        } else if (currentVersion.length() != 0) {
            currentVersionNumber = stoi(currentVersion);
        }

        u8 remoteVersionNumber = 0;
        size_t remotePos = remoteVersion.find('.');
        if (remotePos != std::string::npos) {
            std::string remoteSegment = remoteVersion.substr(0, remotePos);
            remoteVersionNumber = stoi(remoteSegment);
        } else if (remoteVersion.length() != 0) {
            remoteVersionNumber = stoi(remoteVersion);
        }

        if (remoteVersionNumber > currentVersionNumber)
            return true;
        else if (currentVersionNumber > remoteVersionNumber)
            return false;

        std::string currentRemaining = "", remoteRemaining = "";

        if (currentPos != std::string::npos) {
            currentRemaining = currentVersion.substr(currentPos + 1);
        }

        if (remotePos != std::string::npos) {
            remoteRemaining = remoteVersion.substr(remotePos + 1);
        }

        if (currentRemaining.length() + remoteRemaining.length() == 0) {
            return false;
        }
        
        return isNewerVersion(currentRemaining, remoteRemaining);
    }
}
