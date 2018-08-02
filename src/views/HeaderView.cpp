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

#include "HeaderView.hpp"

HeaderView::HeaderView(AssetManager * assetManager) : View(assetManager) {
    title = new TextView(assetManager, assetManager->header_font, "SDFiles Updater", assetManager->text);
    title->frame = { 132, 36, 250, 51 };
    addSubView(title);
}

HeaderView::~HeaderView() {
    if (title != NULL) {
        removeSubView(title);
        delete title;
    }
}

void HeaderView::render(SDL_Rect rect) {
    // Icon
    SDL_Rect iconFrame = { rect.x + 74, rect.y + 29, 30, 44 };
    SDL_RenderCopy(_assetManager->renderer, _assetManager->icon, NULL, &iconFrame);

    // Divider
    _assetManager->setRenderColor(_assetManager->header_footer_divider);
    SDL_RenderDrawLine(_assetManager->renderer, rect.x + 30, rect.y + 87, rect.w - 30, rect.y + 87);

    // Render any subviews
    View::render(rect);
}