// Copyright 2016 Zarklord1 : https://github.com/Zarklord1/WiiU_Save_Manager
#include <string.h>
#include <fcntl.h>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <cstdint>
#include <unistd.h>

#include "ZipUtil.hpp"
#include "Utils.hpp"

#define u32 uint32_t
#define u8 uint8_t

using namespace std;

UnZip::UnZip(const char * zipPath) {
	fileToUnzip = unzOpen(zipPath);
}

UnZip::~UnZip() {
	Close();
}
 
void UnZip::Close() {
	unzClose(fileToUnzip);
}

int UnZip::ExtractFile(const char * internalPath,const char * path) {
	int code = unzLocateFile(fileToUnzip,internalPath,0);
	if(code == UNZ_END_OF_LIST_OF_FILE) 
		return -1;
	
	unz_file_info_s * fileInfo = GetFileInfo();
	
	std::string fullPath(path);
	code = Extract(fullPath.c_str(),fileInfo);
	free(fileInfo);
	return code;
}

int UnZip::ExtractDir(const char * internalDir,const char * externalDir) {
	int i = 0;
	for(;;) {
		int code;
		if(i == 0) {
			code = unzGoToFirstFile(fileToUnzip);
			i++;
		} else {
			code = unzGoToNextFile(fileToUnzip);
		}
		if(code == UNZ_END_OF_LIST_OF_FILE) {
			if(i > 1) {
				return 0;
			} else {
				return -1;
			}
		}
		unz_file_info_s * fileInfo = GetFileInfo();
		
		std::string outputPath = GetFullFileName(fileInfo);
		if(outputPath.find(internalDir,0) != 0) {
			free(fileInfo);
			continue;
		}
		
		outputPath.replace(0,strlen(internalDir),externalDir);
		if(fileInfo->uncompressed_size != 0 && fileInfo->compression_method != 0) {
			i++;
			Extract(outputPath.c_str(),fileInfo);
		}
		free(fileInfo);
	}
}

int UnZip::ExtractAll(const char * dirToExtract) {
	int i = 0;
	for(;;) {
		int code;
		if(i == 0) {
			code = unzGoToFirstFile(fileToUnzip);
			i++;
		} else {
			code = unzGoToNextFile(fileToUnzip);
		}
		if(code == UNZ_END_OF_LIST_OF_FILE) return 0;
		
		unz_file_info_s * fileInfo = GetFileInfo();
		std::string fileName(dirToExtract);
		fileName += '/';
		fileName += GetFullFileName(fileInfo);
		if(fileInfo->uncompressed_size != 0 && fileInfo->compression_method != 0) {
			Extract(fileName.c_str(),fileInfo);
		}
		free(fileInfo);
	}
}

int UnZip::Extract(const char * path, unz_file_info_s * fileInfo) {
	//check to make sure filepath or fileInfo isnt null
	if(path == NULL || fileInfo == NULL)
		return -1;
		
	if(unzOpenCurrentFile(fileToUnzip) != UNZ_OK)
		return -2;
	
	char folderPath[strlen(path)+1];
	strcpy(folderPath,path);
	char * pos = strrchr(folderPath,'/');

	if(pos != NULL) {
		*pos = '\0';
		CreateSubfolder(folderPath);
	}
	
	u32 blocksize = 0x8000;
	u8 * buffer = (u8*)malloc(blocksize);
	if(buffer == NULL)
		return -3;
	u32 done = 0;
	int writeBytes = 0;

	FILE* fp = fopen(path, "w");

	if(fp == NULL) {
		free(buffer);
		return -4;		
	}
		
	while(done < fileInfo->uncompressed_size)
	{
		if(done + blocksize > fileInfo->uncompressed_size) {
			blocksize = fileInfo->uncompressed_size - done;
		}
		unzReadCurrentFile(fileToUnzip, buffer, blocksize);
		writeBytes = write(fileno(fp), buffer, blocksize);
		if(writeBytes <= 0) {
			break;
		}
		done += writeBytes;
	}
	
	fflush(fp);
	fclose(fp);
	
	free(buffer);

	if (done != fileInfo->uncompressed_size)
		return -4;		
	
	unzCloseCurrentFile(fileToUnzip);
	return 0;
}

std::string UnZip::GetFileName(unz_file_info_s * fileInfo) {
	char fileName[fileInfo->size_filename+1];
	std::string path;
	strcpy(fileName,GetFullFileName(fileInfo).c_str());
	char * pos = strrchr(fileName, '/');
	if (pos != NULL) {
		pos++;
		path = pos;
	} else {
		path = fileName;
	}
	return path;
}

std::string UnZip::GetFullFileName(unz_file_info_s * fileInfo) {
	char filePath[fileInfo->size_filename+1];
	unzGetCurrentFileInfo(fileToUnzip,fileInfo,filePath,fileInfo->size_filename,NULL,0,NULL,0);
	filePath[fileInfo->size_filename] = '\0';
	std::string path(filePath);
	path.resize(fileInfo->size_filename);
	return path;
}

unz_file_info_s * UnZip::GetFileInfo() {
	unz_file_info_s * fileInfo = (unz_file_info_s*)malloc(sizeof(unz_file_info_s));
	unzGetCurrentFileInfo(fileToUnzip,fileInfo,NULL,0,NULL,0,NULL,0);
	return fileInfo;
}
