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
#include <string.h>
#include <sys/stat.h>
#include "FileManager.hpp"
#include "microtar/microtar.h"

void FileManager::dealloc() {
    for (u32 i = 0; i < _threads.size(); i++) {
        threadWaitForExit(&_threads.at(i));
        threadClose(&_threads.at(i));
    }
}

bool FileManager::writeFile(string filename, NetRequest * request) {
    if (!deleteFile(filename)) {
        printf("Unable to delete file.");
    }

    FILE * file = fopen(filename.c_str(), "wb");
    if (!file) {
        printf("Unable to open file.");
        return false;
    }

    size_t result = fwrite(request->getData(), sizeof(char), request->getSize(), file);

    fflush(file);
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
        fclose(file);
        return true;
    }

    return false;
}

// http://stackoverflow.com/a/11366985
bool FileManager::createSubfolder(string path) {
	bool bSuccess = false;
    int nRC = ::mkdir(path.c_str(), 0775);
    if(nRC == -1)
    {
        switch(errno)
        {
            case ENOENT:
                //parent didn't exist, try to create it
                if(createSubfolder(path.substr(0, path.find_last_of('/'))))
                    //Now, try to create again.
                    bSuccess = 0 == ::mkdir(path.c_str(), 0775);
                else
                    bSuccess = false;
                break;
            case EEXIST:
                //Done!
                bSuccess = true;
                break;std::string getHost();
            default:
                bSuccess = false;
                break;
        }
    }
    else
        bSuccess = true;
    
    return bSuccess;
}

void FileManager::extract(Tar * tar) {
    _createThread(_extract, tar);
}

Result FileManager::_createThread(ThreadFunc func, void* ptr) {
    Thread thread;
    Result res;

    if (R_FAILED( res = threadCreate(&thread, func, ptr, 0x2000, 0x2B, -2)))
        return res;
    if (R_FAILED( res = threadStart(&thread)))
        return res;

    _threads.push_back(thread);

    return 0;
}

void FileManager::_extract(void * ptr) {
    Tar * tarObj = (Tar *) ptr;

    mtar_t tar;
    mtar_header_t h;
    char * buffer;

    mtar_open(&tar, tarObj->getFilename().c_str(), "r");

    int i = 0;
    int numberOfFiles = tarObj->getNumberOfFiles();
    while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD) {
        mutexLock(&tarObj->mutexRequest);
        tarObj->progress = (i <= numberOfFiles) ? ((double) i / (double) numberOfFiles) : 1;
        mutexUnlock(&tarObj->mutexRequest);

        string path = tarObj->getDestination() + string(h.name);

        // Is the path just a directory?
        if (path.back() == '/') {
            _makeDirectoryParents(path);
            mtar_next(&tar);
            continue;
        }

        // Do we need to create any subdirectories for the file?
        size_t found = path.find_last_of("/");
        if (found != string::npos) {
            _makeDirectoryParents(path.substr(0, found));
        }

        // Create and set the buffer.
        buffer = (char *) calloc(1, h.size + 1);
        if (!buffer) {
            printf("Unable to alloc buffer.\n");
            break;
        }
        mtar_read_data(&tar, buffer, h.size);

        // Open the file pointer.
        FILE * fp = fopen(path.c_str(), "wb");
        if (!fp) {
            printf("Failed to open.\n");
            free(buffer);
            break;
        }

        // Write the buffer to the file pointer
        fwrite(buffer, 1, h.size, fp);

        fflush(fp);
        fclose(fp);
        free(buffer);
        mtar_next(&tar);

        i++;
    }

    mtar_close(&tar);

    mutexLock(&tarObj->mutexRequest);
    tarObj->progress = 1;
    tarObj->isComplete = true;
    tarObj->hasError = false;
    mutexUnlock(&tarObj->mutexRequest);
}

bool FileManager::_makeDirectoryParents(string path)
{
    bool bSuccess = false;
    int nRC = ::mkdir(path.c_str(), 0775);
    if(nRC == -1)
    {
        switch(errno)
        {
            case ENOENT:
                //parent didn't exist, try to create it
                if( _makeDirectoryParents(path.substr(0, path.find_last_of('/'))))
                    //Now, try to create again.
                    bSuccess = 0 == ::mkdir(path.c_str(), 0775);
                else
                    bSuccess = false;
                break;
            case EEXIST:
                //Done!
                bSuccess = true;
                break;std::string getHost();
            default:
                bSuccess = false;
                break;
        }
    }
    else
        bSuccess = true;
    
    return bSuccess;
}
