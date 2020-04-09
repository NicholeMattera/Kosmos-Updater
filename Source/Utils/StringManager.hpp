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

#pragma once

namespace KosmosUpdater::Utils {
    extern const std::string WarningHeader;
    extern const std::string WarningBody;
    extern const std::string WarningFooter;

    extern const std::string AppTitle;
    extern const std::string ActionQuit;

    extern const std::string CheckingKosmosUpdaterUpdate;
    extern const std::string DownloadKosmosUpdaterUpdate;
    extern const std::string SuccessKosmosUpdaterUpdateText;
    extern const std::string SuccessKosmosUpdaterUpdateSubtext;
    extern const std::string ErrorKosmosUpdaterUpdateText;

    class StringManager {
        public:
            static StringManager * Instance();

            std::string getLocalizedString(std::string key);
        
        private:
            libconfig::Config _translations;

            static inline StringManager * _instance = NULL;

            StringManager();
            StringManager(StringManager const&){};

            std::string _getISOLanguageCode();

    };
}
