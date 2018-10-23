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
#include <algorithm>
#include "FileManager.hpp"
#include "ConfigManager.hpp"

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

void FileManager::extract(Zip * zip) {
    _createThread(_extract, zip);
}

Result FileManager::_createThread(ThreadFunc func, Zip * zip) {
    Thread thread;
    Result res;

    if (R_FAILED( res = threadCreate(&thread, func, (void *) zip, 0x2000, 0x2B, -2)))
        return res;
    if (R_FAILED( res = threadStart(&thread)))
        return res;

    zip->thread = thread;
    _threads.push_back(thread);

    return 0;
}

void FileManager::_extract(void * ptr) {
    Zip * zipObj = (Zip *) ptr;
    unzFile unz = unzOpen(zipObj->getFilename().c_str());
    
    int i = 0;
    for (;;) {
        int code;
		if (i == 0) {
			code = unzGoToFirstFile(unz);
			i++;
		} else {
			code = unzGoToNextFile(unz);
		}

		if (code == UNZ_END_OF_LIST_OF_FILE) {
            mutexLock(&zipObj->mutexRequest);
            zipObj->progress = 1;
            zipObj->isComplete = (i > 1);
            zipObj->hasError = (i <= 0);
    
            if (i <= 0) {
                zipObj->errorMessage = "There was no files to extract.";
            }
            mutexUnlock(&zipObj->mutexRequest);
            return;
        } else {
            unz_file_pos pos;
            unzGetFilePos(unz, &pos);

            mutexLock(&zipObj->mutexRequest);
            zipObj->progress = (double) i / (double) zipObj->getNumberOfFiles();
            mutexUnlock(&zipObj->mutexRequest);
        }

        unz_file_info_s * fileInfo = _getFileInfo(unz);

		std::string fileName(zipObj->getDestination());
		fileName += '/';
		fileName += _getFullFileName(unz, fileInfo);

		if (fileInfo->uncompressed_size != 0 && fileInfo->compression_method != 0) {
            int result = _extractFile(fileName.c_str(), unz, fileInfo);

		 	if (result < 0) {
                mutexLock(&zipObj->mutexRequest);
                zipObj->progress = 1;
                zipObj->isComplete = false;
                zipObj->hasError = true;
                zipObj->errorMessage = "There was an error while trying to extract files.";
                mutexUnlock(&zipObj->mutexRequest);

		        free(fileInfo);

                return;
             }
		}

        free(fileInfo);
    }

    mutexLock(&zipObj->mutexRequest);
    zipObj->progress = 1;
    zipObj->isComplete = true;
    zipObj->hasError = false;
    mutexUnlock(&zipObj->mutexRequest);
}

unz_file_info_s * FileManager::_getFileInfo(unzFile unz) {
    unz_file_info_s * fileInfo = (unz_file_info_s*) malloc(sizeof(unz_file_info_s));
    unzGetCurrentFileInfo(unz, fileInfo, NULL, 0, NULL, 0, NULL, 0);
    return fileInfo;
}

string FileManager::_getFullFileName(unzFile unz, unz_file_info_s * fileInfo) {
    char filePath[fileInfo->size_filename + 1];
    
    unzGetCurrentFileInfo(unz, fileInfo, filePath, fileInfo->size_filename, NULL, 0, NULL, 0);
    filePath[fileInfo->size_filename] = '\0';
    
    string path(filePath);
    path.resize(fileInfo->size_filename);

    return path;
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

int FileManager::_extractFile(const char * path, unzFile unz, unz_file_info_s * fileInfo) {
	//check to make sure filepath or fileInfo isnt null
	if (path == NULL || fileInfo == NULL)
		return -1;
		
	if (unzOpenCurrentFile(unz) != UNZ_OK)
		return -2;
	
	char folderPath[strlen(path) + 1];
	strcpy(folderPath, path);
	char * pos = strrchr(folderPath, '/');
	if (pos != NULL) {
		*pos = '\0';
		_makeDirectoryParents(string(folderPath));
	}
	
	u32 blocksize = 0x8000;
	u8 * buffer = (u8*) malloc(blocksize);
	if (buffer == NULL)
		return -3;
	u32 done = 0;
	int writeBytes = 0;
	FILE * fp = fopen(path, "w");
	if (fp == NULL) {
		free(buffer);
		return -4;		
	}
		
	while (done < fileInfo->uncompressed_size) {
		if (done + blocksize > fileInfo->uncompressed_size) {
			blocksize = fileInfo->uncompressed_size - done;
		}
		unzReadCurrentFile(unz, buffer, blocksize);
		writeBytes = write(fileno(fp), buffer, blocksize);
		if (writeBytes <= 0) {
			break;
		}
		done += writeBytes;
	}
	
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
	
	free(buffer);
	if (done != fileInfo->uncompressed_size)
		return -4;		
	
	unzCloseCurrentFile(unz);
	return 0;
}
