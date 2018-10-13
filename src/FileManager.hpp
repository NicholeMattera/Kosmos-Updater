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

#pragma once

#include <string>
#include <vector>
#include "models/NetRequest.hpp"
#include "models/Unzip.hpp"
#include "minizip/unzip.h"

using namespace std;

class FileManager {
    public:
        static void dealloc();
        static bool writeFile(string filename, NetRequest * request);
        static bool deleteFile(string filename);
        static bool fileExists(string filename);
        static bool createSubfolder(string path);
        static Unzip * unzipArchive(string filename, string dest);

    private:
        static inline vector<Thread> _threads;

        static Result _createThread(ThreadFunc func, void* ptr);
        static void _unzip(void * ptr);
        static unz_file_info_s * _getFileInfo(unzFile unz);
        static std::string _getFullFileName(unzFile unz, unz_file_info_s * fileInfo);
        static int Extract(const char * path, unzFile unz, unz_file_info_s * fileInfo);
};
