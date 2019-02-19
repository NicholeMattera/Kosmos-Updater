// Kosmos Updater
// Copyright (C) 2019 Steven Mattera
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

#include <SDL2/SDL2_gfxPrimitives.h>
#include "PasscodeView.hpp"
#include "../AssetManager.hpp"
#include "../SceneDirector.hpp"

PasscodeView::PasscodeView() {
    _buttonTimeout = 0;
}

PasscodeView::~PasscodeView() {
}

void PasscodeView::handleButton(u32 buttons, double dTime) {
    if (_buttonTimeout = 0 && _passcode.size() < 4) {
        if (buttons & KEY_PLUS) {
            dismiss(false);
        }

        if (buttons & KEY_A) {
            _passcode.push_back(KEY_A);
            _buttonTimeout = 1;
        }
        else if (buttons & KEY_B) {
            _passcode.push_back(KEY_B);
            _buttonTimeout = 1;
        }
        else if (buttons & KEY_X) {
            _passcode.push_back(KEY_X);
            _buttonTimeout = 1;
        }
        else if (buttons & KEY_Y) {
            _passcode.push_back(KEY_Y);
            _buttonTimeout = 1;
        }

        if (_buttonTimeout == 1 && _passcode.size() == 4) {
            if (_validatePasscode()) {
                dismiss(true);
            } else {
                _passcode.clear();
            }
        }
    } else {
        _buttonTimeout += dTime;
        if (_buttonTimeout >= 500) {
            _buttonTimeout = 0;
        }
    }
}

void PasscodeView::render(SDL_Rect rect, double dTime) {
    // Draw background.
    roundedBoxRGBA(
        SceneDirector::renderer,
        255,
        0,
        1025,
        0, 
        4,
        AssetManager::background.r, AssetManager::background.g, AssetManager::background.b, AssetManager::background.a);

    // Render any subviews.
    View::render(rect, dTime);
}

bool PasscodeView::_validatePasscode() {
    int i = 0;
    for (auto const& key : _passcode) {
        if (key != BIT(PASSCODE[i])) {
            return false;
        }
        i++;
    }

    return true;
}