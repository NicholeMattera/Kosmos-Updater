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
#include "FooterView.hpp"

FooterView::FooterView(AssetManager * assetManager) : View(assetManager) {
    _aButtonTexture = NULL;
    _bButtonTexture = NULL;
    _xButtonTexture = NULL;
    _yButtonTexture = NULL;
}

FooterView::~FooterView() {}

void FooterView::render(SDL_Rect rect) {
    // Divider.
    _assetManager->setRenderColor(_assetManager->header_footer_divider);
    SDL_RenderDrawLine(_assetManager->renderer, rect.x + 30, rect.y, rect.w - 30, rect.y);

    // Render Actions.
    int current_x = rect.w - 60;
    for (list<Action *>::iterator it = actions.begin(); it != actions.end(); it++) {
        Action * action = (*it);

        // Create texture if it doesn't already exists.
        if (action->textTexture == NULL) {
            SDL_Surface *surface = TTF_RenderText_Blended(_assetManager->footer_font, action->text.c_str(), _assetManager->text);
            action->textTexture = SDL_CreateTextureFromSurface(_assetManager->renderer, surface);
            action->textWidth = surface->w;
            action->textHeight = surface->h;
            SDL_FreeSurface(surface);
        }

        // Render Action Text
        current_x -= action->textWidth;
        SDL_Rect textFrame = { current_x, rect.y + 25, action->textWidth, action->textHeight };
        SDL_RenderCopy(_assetManager->renderer, action->textTexture, NULL, &textFrame);

        current_x -= 37;
        // Render Action Button Icon
        SDL_Rect iconFrame = { current_x, rect.y + 25, 25, 25 };
        switch(action->button) {
            case B_BUTTON:
                _renderButton(action->button, _bButtonTexture, iconFrame);
                break;

            case X_BUTTON:
                _renderButton(action->button, _xButtonTexture, iconFrame);
                break;

            case Y_BUTTON:
                _renderButton(action->button, _yButtonTexture, iconFrame);
                break;

            default:
                _renderButton(action->button, _aButtonTexture, iconFrame);
                break;
        }
        current_x -= 40;
    }

    // Render any subviews.
    View::render(rect);
}

void FooterView::_renderButton(ActionButton button, SDL_Texture * texture, SDL_Rect frame) {
    if (texture == NULL) {
        u16 buttonText;
        switch(button) {
            case B_BUTTON:
                buttonText = 0xE0E1;
                break;

            case X_BUTTON:
                buttonText = 0xE0E2;
                break;

            case Y_BUTTON:
                buttonText = 0xE0E3;
                break;

            default:
                buttonText = 0xE0E0;
                break;
        }

        SDL_Surface *surface = TTF_RenderGlyph_Blended(_assetManager->button_font, buttonText, _assetManager->text);
        texture = SDL_CreateTextureFromSurface(_assetManager->renderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_RenderCopy(_assetManager->renderer, texture, NULL, &frame);
}