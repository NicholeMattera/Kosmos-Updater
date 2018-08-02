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

#include "TextView.hpp"

TextView::TextView(AssetManager * assetManager, TTF_Font * theFont, string theText, SDL_Color theTextColor) : View(assetManager) {
    font = theFont;
    text = theText;
    textColor = theTextColor;
}

TextView::~TextView() {
    if (_textTexture != NULL)
        SDL_DestroyTexture(_textTexture);
}

void TextView::render(SDL_Rect rect) {
    // Create texture if it doesn't already exists.
    if (_textTexture == NULL) {
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        _textTexture = SDL_CreateTextureFromSurface(_assetManager->renderer, surface);
        _textWidth = surface->w;
        _textHeight = surface->h;
        SDL_FreeSurface(surface);
    }

    // Render the text.
    SDL_Rect textFrame = { rect.x, rect.y, min(_textWidth, rect.w), min(_textHeight, rect.h) };
    SDL_RenderCopy(_assetManager->renderer, _textTexture, NULL, &textFrame);

    // Render any subviews.
    View::render(rect);
}

void TextView::setFont(TTF_Font * theFont) {
    font = theFont;
    _reset();
}

void TextView::setText(string theText) {
    text = theText;
    _reset();
}

void TextView::setTextColor(SDL_Color theTextColor) {
    textColor = theTextColor;
    _reset();
}

void TextView::_reset() {
    if (_textTexture != NULL) {
        SDL_DestroyTexture(_textTexture);
        _textTexture = NULL;
    }

    _textWidth = 0;
    _textHeight = 0;
}