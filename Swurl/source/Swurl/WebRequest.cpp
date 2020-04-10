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

#include <switch.h>

#include "SessionManager.hpp"

using namespace std;

namespace swurl {
    WebRequest::WebRequest(std::string url) : WebRequest(GET, url) {}
    
    WebRequest::WebRequest(WebRequestMethod method, std::string url) {
        this->method = method;
        this->url = url;
        this->sslVerifyHost = true;
    }
    
    WebRequest::WebRequest(WebRequestMethod method, std::string url, std::map<std::string, std::string> headers) : WebRequest(method, url) {
        this->headers = headers;
    }
}
