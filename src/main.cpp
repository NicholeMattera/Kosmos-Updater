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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <switch.h>

#include "AssetManager.hpp"
#include "SceneDirector.hpp"

using namespace std;

int main(int argc, char **argv)
{
    socketInitializeDefault();
    nxlinkStdio();

    AssetManager * assetManager = new AssetManager();
    if(!assetManager->initialize()) {
        delete assetManager;
        return 0;
    }

    SceneDirector * sceneDirector = new SceneDirector(assetManager);

    // Main Game Loop
    while(appletMainLoop())
    {
        if (!sceneDirector->direct())
            break;
    }

    delete sceneDirector;
    delete assetManager;

    socketExit();

    return 0;
}
