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

#include <string.h>
#include "NetRequest.hpp"

NetRequest::NetRequest(string method, string url) {
    mutexInit(&mutexRequest);

    _method = method;
    _url = url;
    _size = 0;
    _data = (char *) malloc(1);
    _headerSize = 0;
    _headerData = (char *) malloc(1);

    progress = 0.f;
    isComplete = false;
    hasError = false;
    errorMessage = "";
    bundle = "";
    channel = "";
}

NetRequest::~NetRequest() {
    free(_data);
    free(_headerData);
}

string NetRequest::getMethod() {
    return _method;
}

string NetRequest::getURL() {
    return _url;
}

size_t NetRequest::getSize() {
    return _size;
}

char * NetRequest::getData() {
    return _data;
}

size_t NetRequest::getHeaderSize() {
    return _headerSize;
}

char * NetRequest::getHeaderData() {
    return _headerData;
}

size_t NetRequest::appendData(void *contents, size_t size, size_t nmemb) {
    size_t realsize = size * nmemb;
    mutexLock(&mutexRequest);

    _data = (char *) realloc(_data, _size + realsize + 1);
    if (_data == NULL) {
        hasError = true;
        errorMessage = "Not enough memory.";
        return 0;
    }

    memcpy(&(_data[_size]), contents, realsize);
    _size += realsize;
    _data[_size] = 0;

    mutexUnlock(&mutexRequest);

    return realsize;
}

size_t NetRequest::appendHeaderData(void *contents, size_t size, size_t nmemb) {
    size_t realsize = size * nmemb;
    mutexLock(&mutexRequest);

    _headerData = (char *) realloc(_headerData, _headerSize + realsize + 1);
    if (_headerData == NULL) {
        hasError = true;
        errorMessage = "Not enough memory.";
        return 0;
    }

    memcpy(&(_headerData[_headerSize]), contents, realsize);
    _headerSize += realsize;
    _headerData[_headerSize] = 0;

    mutexUnlock(&mutexRequest);

    return realsize;
}
