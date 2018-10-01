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

#pragma once

#include <switch.h>
#include <string>
#include <functional>

using namespace std;

class NetRequest {
    public:
        Mutex mutexRequest;
        string method;
        string url;
        size_t size;
        char * data;
        double progress;
        bool isComplete;
        bool hasError;
        string errorMessage;

        NetRequest(string method, string url);
        ~NetRequest();
};
