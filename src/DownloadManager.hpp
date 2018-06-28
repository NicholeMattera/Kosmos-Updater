// SDFile Updater
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

#pragma once

#include <string>

#include "Download.hpp"

class DownloadManager {
    public:
        DownloadManager(std::string host);
        ~DownloadManager();
        Download * getLatestVersion(std::string channel);
        Download * getLatestFiles(std::string channel);

    private:
        std::string hostname;
        static size_t writeData(void *contents, size_t size, size_t nmemb, void *userp);
};
