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

void FileManager::dealloc() {
    for (u32 i = 0; i < _threads.size(); i++) {
        threadWaitForExit(&_threads.at(i));
        threadClose(&_threads.at(i));
    }
}

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

Unzip * FileManager::unzipArchive(string filename, string dest) {
    Unzip * unzip = new Unzip(filename, dest);
    _createThread(_unzip, unzip);
    return unzip;
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

void FileManager::_unzip(void * ptr) {
    Unzip * unzip = (Unzip *) ptr;
    unzFile unz = unzOpen(unzip->getFilename().c_str());

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
            mutexLock(&unzip->mutexRequest);
            unzip->progress = 1;
            unzip->isComplete = (i > 1);
            unzip->hasError = (i <= 0);
    
            if (i <= 0) {
                unzip->errorMessage = "There was no files to extract.";
            }

            mutexUnlock(&unzip->mutexRequest);
            return;
        } else {
            unz_file_pos pos;
            unzGetFilePos(unz, &pos);
            printf("unzGetFilePos: (%ld, %ld)\n", pos.pos_in_zip_directory, pos.num_of_file);

            mutexLock(&unzip->mutexRequest);
            unzip->progress = (double) pos.pos_in_zip_directory / (double) pos.num_of_file;
            mutexUnlock(&unzip->mutexRequest);
        }
		
		unz_file_info_s * fileInfo = _getFileInfo(unz);
		std::string fileName(unzip->getDestination());
		fileName += '/';
		fileName += _getFullFileName(unz, fileInfo);
		if (fileInfo->uncompressed_size != 0 && fileInfo->compression_method != 0) {
            printf("Filename: %s\n", fileName.c_str());
            int result = Extract(fileName.c_str(), unz, fileInfo);
            printf("Result: %d\n", result);

		 	if (result < 0) {
                mutexLock(&unzip->mutexRequest);
                unzip->progress = 1;
                unzip->isComplete = false;
                unzip->hasError = true;
                unzip->errorMessage = "There was an error while trying to extract files.";
                mutexUnlock(&unzip->mutexRequest);

		        free(fileInfo);

                return;
             }
		}

		free(fileInfo);
    }
}

unz_file_info_s * FileManager::_getFileInfo(unzFile unz) {
	unz_file_info_s * fileInfo = (unz_file_info_s*) malloc(sizeof(unz_file_info_s));
	unzGetCurrentFileInfo(unz, fileInfo, NULL, 0, NULL, 0, NULL, 0);
	return fileInfo;
}

std::string FileManager::_getFullFileName(unzFile unz, unz_file_info_s * fileInfo) {
	char filePath[fileInfo->size_filename + 1];
	unzGetCurrentFileInfo(unz, fileInfo, filePath, fileInfo->size_filename, NULL, 0, NULL, 0);
	filePath[fileInfo->size_filename] = '\0';

	std::string path(filePath);
	path.resize(fileInfo->size_filename);
    
	return path;
}

int FileManager::Extract(const char * path, unzFile unz, unz_file_info_s * fileInfo) {
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
		createSubfolder(string(folderPath));
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
		
	while (done < fileInfo->uncompressed_size)
	{
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
