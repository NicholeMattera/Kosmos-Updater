#pragma once
	
// use included minizip library
#include "minizip/unzip.h"

#include <string>

class UnZip {
	public:		
		unzFile fileToUnzip;

		UnZip(const char * zipPath);
		~UnZip();
		void Close();
		int ExtractFile(const char * internalPath,const char * path);
		int ExtractAll(const char * dirToExtract);
		int ExtractDir(const char * internalDir,const char * externalDir);
		std::string GetFullFileName(unz_file_info_s * fileInfo);
		unz_file_info_s * GetFileInfo();
	private:
		int Extract(const char * path, unz_file_info_s * fileInfo);
		std::string GetFileName(unz_file_info_s * fileInfo);
};

