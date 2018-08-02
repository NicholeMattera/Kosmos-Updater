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

FooterView::FooterView(AssetManager * assetManager) : View(assetManager) {}

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
            SDL_Surface *surface = TTF_RenderText_Blended(_assetManager->action_font, action->text.c_str(), _assetManager->text);
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
        SDL_Rect iconFrame;
        switch(action->button) {
            case A_BUTTON:
                iconFrame = { current_x, rect.y + 25, 25, 25 };
                SDL_RenderCopy(_assetManager->renderer, _assetManager->a_button, NULL, &iconFrame);
                break;

            case B_BUTTON:
                break;

            case X_BUTTON:
                break;

            case Y_BUTTON:
                break;
        }
        current_x -= 12;
    }

    // Render any subviews.
    View::render(rect);
}