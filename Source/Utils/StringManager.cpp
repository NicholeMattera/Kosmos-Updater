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

#include "StringManager.hpp"

#define DEFAULT_LANGUAGE "en-us"

namespace KosmosUpdater::Utils {
    extern const std::string WarningHeader = "WarningHeader";
    extern const std::string WarningBody = "WarningBody";
    extern const std::string WarningFooter = "WarningFooter";

    extern const std::string AppTitle = "AppTitle";
    extern const std::string ActionQuit = "ActionQuit";

    extern const std::string CheckingKosmosUpdaterUpdate = "CheckingKosmosUpdaterUpdate";
    extern const std::string DownloadKosmosUpdaterUpdate = "DownloadKosmosUpdaterUpdate";
    extern const std::string SuccessKosmosUpdaterUpdateText = "SuccessKosmosUpdaterUpdateText";
    extern const std::string SuccessKosmosUpdaterUpdateSubtext = "SuccessKosmosUpdaterUpdateSubtext";
    extern const std::string ErrorKosmosUpdaterUpdateText = "ErrorKosmosUpdaterUpdateText";

    StringManager * StringManager::Instance() {
        if (_instance == NULL) {
            _instance = new StringManager;
        }

        return _instance;
    }

    StringManager::StringManager() {
        auto path = std::string("romfs:/lang.") + _getISOLanguageCode() + std::string(".cfg");

        try {
            _translations.readFile(path.c_str());
        } catch (...) {
            path = std::string("romfs:/lang.") + std::string(DEFAULT_LANGUAGE) + std::string(".cfg");
            _translations.readFile(path.c_str());
        }
    }

    std::string StringManager::getLocalizedString(std::string key) {
        try {
            return _translations.lookup(key);
        } catch (...) {
            return "";
        }
    }
    
    std::string StringManager::_getISOLanguageCode() {
        u64 LanguageCode = 0;
        SetLanguage Language = SetLanguage_JA;

        Result rc = setGetSystemLanguage(&LanguageCode);
        if (R_FAILED(rc))
            return DEFAULT_LANGUAGE;

        rc = setMakeLanguage(LanguageCode, &Language);
        if (R_FAILED(rc))
            return DEFAULT_LANGUAGE;

        switch (Language) {
            case SetLanguage_JA:
                return "ja";
            
            case SetLanguage_ENUS:
                return "en-us";

            case SetLanguage_FR:
                return "fr";

            case SetLanguage_DE:
                return "de";

            case SetLanguage_IT:
                return "it";

            case SetLanguage_ES:
                return "es";

            case SetLanguage_ZHCN:
                return "zh-cn";

            case SetLanguage_KO:
                return "ko";

            case SetLanguage_NL:
                return "nl";

            case SetLanguage_PT:
                return "pt";

            case SetLanguage_RU:
                return "ru";

            case SetLanguage_ZHTW:
                return "zh-tw";

            case SetLanguage_ENGB:
                return "en-gb";

            case SetLanguage_FRCA:
                return "fr-ca";

            case SetLanguage_ES419:
                return "es-419";

            default:
                return DEFAULT_LANGUAGE;
        }
    }
}
