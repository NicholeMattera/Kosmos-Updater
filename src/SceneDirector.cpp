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

#include <switch.h>
#include "SceneDirector.hpp"

SceneDirector::SceneDirector(AssetManager * assetManager) {
    _now = SDL_GetPerformanceCounter();
    _last = 0;
    _assetManager = assetManager;
    /* Temporary for testing. Views belong in scenes. */
    _headerView = NULL;
    _footerView = NULL;
    /* End of temporary for testing. */
}

SceneDirector::~SceneDirector() {
    /* Temporary for testing. Views belong in scenes. */
    if (_headerView != NULL)
        delete _headerView;

    if (_footerView != NULL)
        delete _footerView;
    /* End of temporary for testing. */
}

bool SceneDirector::direct() {
    _last = _now;
    _now = SDL_GetPerformanceCounter();
    double dTime = (double) ((_now - _last) * 1000 / SDL_GetPerformanceFrequency());

    hidScanInput();
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (kDown & KEY_A)
        return false;

    _assetManager->setRenderColor(_assetManager->background);
    SDL_RenderClear(_assetManager->renderer);

    /* Temporary for testing. Views belong in scenes. */
    if (_headerView == NULL)
        _headerView = new HeaderView(_assetManager);

    if (_footerView == NULL) {
        _footerView = new FooterView(_assetManager);
        _footerView->actions.push_front(new Action(A_BUTTON, "OK"));
    }

    _headerView->render({ 0, 0, 1280, 88 });
    _footerView->render({ 0, 647, 1280, 73 });
    /* End of temporary for testing. */

    SDL_RenderPresent(_assetManager->renderer);
    
    return true;
};
