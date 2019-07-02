// Kosmos Updater
// Copyright (C) 2019 Steven Mattera
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

#include "ThreadObj.hpp"

namespace ku::models {
    class NetRequest : public ThreadObj {
        public:
            std::string bundle;
            std::string channel;

            NetRequest(std::string method, std::string url);
            ~NetRequest();
            std::string getMethod();
            std::string getURL();
            size_t getSize();
            char * getData();
            std::string getVersionNumber();
            std::string getNumberOfFiles();
            size_t appendData(void *contents, size_t size, size_t nmemb);
            size_t appendHeaderData(void *contents, size_t size, size_t nmemb);

        private:
            std::string _method;
            std::string _url;
            size_t _size;
            char * _data;
            size_t _headerSize;
            char * _headerData;

            std::string _versionNumber;
            std::string _numberOfFiles;
    };
}
