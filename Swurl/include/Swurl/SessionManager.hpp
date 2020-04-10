/*
 * Swurl
 * Copyright (c) 2019 Nichole Mattera
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <string>
#include <map>
#include <functional>
#include <curl/curl.h>

#include "WebRequest.hpp"

namespace swurl {
    class SessionManager {
        public:
            static std::string proxyUrl;
            static std::string proxyUsername;
            static std::string proxyPassword;
            static std::string userAgent;
            static std::map<std::string, std::string> requestHeaders;

            static std::function<void(WebRequest *, double)> onProgressChanged;
            static std::function<void(WebRequest *)> onCompleted;
            static std::function<void(WebRequest *, std::string)> onError;

            static void initialize();
            static void dealloc();
            static void makeRequest(WebRequest * request);
        
        private:
            static curl_slist * _generateHeaders(WebRequest * request);
            static std::string _getMethod(WebRequest * request);
            static size_t _writeHeader(const char * in, std::size_t size, std::size_t num, WebRequest * request);
            static size_t _write(const char* in, std::size_t size, std::size_t num, WebRequest * request);
            static size_t _progress(WebRequest * request, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
            static void _parseResponseHeader(WebRequest * request);
    };
}
