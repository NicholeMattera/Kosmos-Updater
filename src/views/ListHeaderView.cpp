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

#include "ListHeaderView.hpp"
#include "../SceneDirector.hpp"

ListHeaderView::ListHeaderView(string text) : View() {
    _text = text;
    _textTexture = NULL;
    frame = { 0, 0, 840, 80 };
}

ListHeaderView::~ListHeaderView() {
    if (_textTexture != NULL)
        SDL_DestroyTexture(_textTexture);
}

void ListHeaderView::render(SDL_Rect rect, double dTime) {
    // Draw rectangle.
    AssetManager::setRenderColor(AssetManager::list_divider);
    SDL_Rect squareFrame = { rect.x + 5, rect.y + 44, 5, 22 };
    SDL_RenderFillRect(SceneDirector::renderer, &squareFrame);

    // Draw header.
    if (_textTexture == NULL) {
        SDL_Surface *surface = TTF_RenderText_Blended(AssetManager::subbody_font, _text.c_str(), AssetManager::text);
        _textTexture = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
        _textWidth = surface->w;
        _textHeight = surface->h;
        SDL_FreeSurface(surface);
    }

    SDL_Rect textFrame = { rect.x + 20, rect.y + 47, _textWidth, _textHeight };
    SDL_RenderCopy(SceneDirector::renderer, _textTexture, NULL, &textFrame);
}
