// Kosmos Updater
// Copyright (C) 2019 Nichole Mattera
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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <switch.h>
#include <Swurl.hpp>

#include "AssetManager.hpp"
#include "ConfigManager.hpp"
#include "SceneDirector.hpp"

using namespace ku;
using namespace std;
using namespace swurl;

int main(int argc, char **argv)
{
    SessionManager::initialize();
    SessionManager::userAgent = string("kosmos-updater/") + VERSION;
    SessionManager::requestHeaders.insert(
        pair<string, string>(
            "Cache-Control",
            "no-cache"
        )
    );

    #ifdef DEBUG
        nxlinkStdio();
    #endif

    ConfigManager::initialize();

    if (ConfigManager::shouldUseProxy()) {
        SessionManager::proxyUrl = ConfigManager::getProxy();
        SessionManager::proxyUsername = ConfigManager::getProxyUsername();
        SessionManager::proxyPassword = ConfigManager::getProxyPassword();
    }

    SceneDirector * sceneDirector = new SceneDirector();
    if (!SceneDirector::renderer || !SceneDirector::window) {
        return -1;
    }

    if (!AssetManager::initialize()) {
        AssetManager::dealloc();
        return -1;
    }

    // Main Game Loop
    while (appletMainLoop())
    {
        if (!sceneDirector->direct())
            break;
    }

    AssetManager::dealloc();
    delete sceneDirector;
    ConfigManager::dealloc();
    SessionManager::dealloc();

    return 0;
}
