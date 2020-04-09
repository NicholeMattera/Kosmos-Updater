/*
 * Kosmos Updater
 * Copyright (c) 2020 Nichole Mattera
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <curl/curl.h>
#include <memory>
#include <switch.h>

#include "Application.hpp"
#include "Scenes/Root.hpp"

extern "C" {
    void userAppInit(void);
    void userAppExit(void);
}

void userAppInit(void) {
    if (R_FAILED(setsysInitialize()))
        fatalThrow(0x9001);

    if (R_FAILED(socketInitializeDefault()))
        fatalThrow(0x9002);
        
    #ifdef DEBUG
        nxlinkStdio();
    #endif

    // res = nifmInitialize(NifmServiceType_User);
    // if (R_FAILED(res))
    //     fatalThrow(res);

    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
        fatalThrow(0x9004);

    if (R_FAILED(romfsInit()))
        fatalThrow(0x9005);

    // rc = plInitialize();
    // if (R_FAILED(res))
    //     fatalThrow(res);
}

void userAppExit(void) {
    // plExit();
    romfsExit();
    curl_global_cleanup();
    // nifmExit();
    socketExit();
    setsysExit();
}

int main(int argc, char * argv[]) {
    std::unique_ptr<KosmosUpdater::Application> app = std::make_unique<KosmosUpdater::Application>();
    std::shared_ptr<KosmosUpdater::Scenes::Root> root = std::make_shared<KosmosUpdater::Scenes::Root>();
    return app->start(root);
}
