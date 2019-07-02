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

#include <minizip/unzip.h>
#include <string>
#include <vector>

#include "models/NetRequest.hpp"
#include "models/ThreadObj.hpp"
#include "models/Zip.hpp"

namespace ku {
    class FileManager {
        public:
            static bool writeFile(std::string filename, models::NetRequest * request);
            static bool deleteFile(std::string filename);
            static bool fileExists(std::string filename);
            static bool createSubfolder(std::string path);
            static void extract(models::Zip * zip);
            static void cleanUpFiles(models::ThreadObj * status);
            static void applyNoGC(models::ThreadObj * status);

        private:
            static inline std::vector<Thread> _threads;

            static Result _createThread(ThreadFunc func, models::ThreadObj * arg);
            static void _extract(void * ptr);
            static void _cleanUpFiles(void * ptr);
            static void _applyNoGC(void * ptr);
            static unz_file_info_s * _getFileInfo(unzFile unz);
            static std::string _getFullFileName(unzFile unz, unz_file_info_s * fileInfo);
            static bool _makeDirectoryParents(std::string path);
            static int _extractFile(const char * path, unzFile unz, unz_file_info_s * fileInfo);

            static inline const std::string HEKATE_FILE = "sdmc:/bootloader/hekate_ipl.ini";
    };
}
