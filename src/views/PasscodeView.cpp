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

#include <sstream>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "PasscodeView.hpp"
#include "../AssetManager.hpp"
#include "../SceneDirector.hpp"

PasscodeView::PasscodeView() {
    _height = 256;

    _titleTextView = new TextView(AssetManager::header_font, "Enter Passcode", AssetManager::text);
    _titleTextView->textAlignment = CENTER_ALIGN;
    _height += _titleTextView->getTextHeight();
    addSubView(_titleTextView);

    _messageTextView = new TextView(AssetManager::body_font, "Use A, B, X, and Y to enter in the passcode or press + to exit.", AssetManager::text);
    _messageTextView->textAlignment = CENTER_ALIGN;
    _messageTextView->lineHeight = 32;
    _height += _messageTextView->getTextHeight();
    addSubView(_messageTextView);

    SDL_Surface * surface;
    surface = TTF_RenderGlyph_Blended(AssetManager::large_button_font, 0xE0E0, AssetManager::text);
    _aButton = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderGlyph_Blended(AssetManager::large_button_font, 0xE0E1, AssetManager::text);
    _bButton = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderGlyph_Blended(AssetManager::large_button_font, 0xE0E2, AssetManager::text);
    _xButton = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderGlyph_Blended(AssetManager::large_button_font, 0xE0E3, AssetManager::text);
    _yButton = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderGlyph_Blended(AssetManager::large_button_font, 0xE0F2, AssetManager::text);
    _minusButton = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
    SDL_FreeSurface(surface);

    _yPosition = (720 - _height) / 2;
    _titleTextView->frame = { 255, _yPosition + 55, 770, 1 };
    _messageTextView->frame = { 255, _yPosition + 93 + _titleTextView->getTextHeight(), 770, 1 };
}

PasscodeView::~PasscodeView() {
    if (_titleTextView != NULL)
        delete _titleTextView;

    if (_messageTextView != NULL)
        delete _messageTextView;

    if (_aButton != NULL)
        SDL_DestroyTexture(_aButton);

    if (_bButton != NULL)
        SDL_DestroyTexture(_bButton);

    if (_xButton != NULL)
        SDL_DestroyTexture(_xButton);

    if (_yButton != NULL)
        SDL_DestroyTexture(_yButton);

    if (_minusButton != NULL)
        SDL_DestroyTexture(_minusButton);
}

void PasscodeView::handleButton(u32 buttons, double dTime) {
    if (_passcode.size() < 4) {
        if (buttons & KEY_PLUS) {
            _passcode.clear();
            dismiss(false);
        }

        if (buttons & KEY_A) {
            _passcode.push_back(KEY_A);
        }
        else if (buttons & KEY_B) {
            _passcode.push_back(KEY_B);
        }
        else if (buttons & KEY_X) {
            _passcode.push_back(KEY_X);
        }
        else if (buttons & KEY_Y) {
            _passcode.push_back(KEY_Y);
        }

        if (_passcode.size() == 4) {
            if (_validatePasscode()) {
                _passcode.clear();
                dismiss(true);
            } else {
                _passcode.clear();
            }
        }
    }
}

void PasscodeView::render(SDL_Rect rect, double dTime) {
    // Draw background.
    roundedBoxRGBA(
        SceneDirector::renderer,
        255,
        _yPosition,
        1025,
        _yPosition + _height, 
        4,
        AssetManager::background.r, AssetManager::background.g, AssetManager::background.b, AssetManager::background.a);

    int i = 0;
    for (auto const& key : _passcode) {
        SDL_Rect iconFrame = { 310 + (165 * i) + 47, _yPosition + 131 + _titleTextView->getTextHeight() + _messageTextView->getTextHeight(), 70, 70 };
        switch (key) {
            case KEY_A:
                SDL_RenderCopy(SceneDirector::renderer, _aButton, NULL, &iconFrame);
                break;

            case KEY_B:
                SDL_RenderCopy(SceneDirector::renderer, _bButton, NULL, &iconFrame);
                break;

            case KEY_X:
                SDL_RenderCopy(SceneDirector::renderer, _xButton, NULL, &iconFrame);
                break;

            case KEY_Y:
                SDL_RenderCopy(SceneDirector::renderer, _yButton, NULL, &iconFrame);
                break;
        }
        i++;
    }

    for (i = i; i < 4; i++) {
        SDL_Rect iconFrame = { 310 + (165 * i) + 47, _yPosition + 131 + _titleTextView->getTextHeight() + _messageTextView->getTextHeight(), 70, 70 };
        SDL_RenderCopy(SceneDirector::renderer, _minusButton, NULL, &iconFrame);
    }

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