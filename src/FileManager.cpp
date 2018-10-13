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

#include <unistd.h>
#include "FileManager.hpp"

bool FileManager::writeFile(string filename, NetRequest * request) {
    deleteFile(filename);

    FILE * file = fopen(filename.c_str(), "wb");
    if (!file) {
        return false;
    }

    size_t result = fwrite(request->getData(), sizeof(char), request->getSize(), file);

    fflush(file);
    fsync(fileno(file));
    fclose(file);

    return (result == request->getSize());
}

bool FileManager::deleteFile(string filename) {
    if (fileExists(filename)) {
        return remove(filename.c_str()) == 0;
    }

    return false;
}

bool FileManager::fileExists(string filename) {
    FILE * file = fopen(filename.c_str(), "r");

    if (file) {
        fflush(file);
        fsync(fileno(file));
        fclose(file);
        return true;
    }

    return false;
}
