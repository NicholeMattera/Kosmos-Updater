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

#include "ImageView.hpp"
#include "../Application.hpp"

using namespace KosmosUpdater;

namespace KosmosUpdater::Views {
    ImageView::ImageView(SDL_Texture * image) {
        _image = image;
    }

    void ImageView::onRender(SDL_Rect rect, double dTime) {
        View::render(rect, dTime);
        
        SDL_RenderCopy(Application::renderer, _image, NULL, &rect);
    }
}