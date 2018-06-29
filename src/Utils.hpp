#pragma once

#include <stdio.h>
#include <string>

#include "Download.hpp"

bool writeFile(std::string filename, Download * download);
bool deleteFile(std::string filename);
bool fileExists(std::string filename);

// folder stuff
bool mkpath( std::string path );
bool CreateSubfolder( char* cstringpath );
