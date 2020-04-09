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

#include "Application.hpp"
#include "Draw.hpp"

namespace KosmosUpdater {
    void Draw::pixel(u16 x, u16 y, Color color) {
        u32 position = y * Application::Stride + x * sizeof(u32);
        u8 * framebuffer = Application::CurrentFramebuffer;

        framebuffer[position] = Draw::_blend(framebuffer[position], color.red, color.alpha);
        framebuffer[position + 1] = Draw::_blend(framebuffer[position + 1], color.green, color.alpha);
        framebuffer[position + 2] = Draw::_blend(framebuffer[position + 2], color.blue, color.alpha);
        framebuffer[position + 3] = 255;
    }
    
    void Draw::fill(Rect rect, Color color) {
        for (u16 x = rect.x; x < rect.x + rect.width; x++) {
            for (u16 y = rect.y; y < rect.y + rect.height; y++) {
                Draw::pixel(x, y, color);
            }
        }
    }

    void Draw::image(u16 x, u16 y, std::shared_ptr<Image> image) {
        if (!image->imageLoaded || image->rawImage == nullptr)
            return;

        for (u16 x2 = x; x2 < x + image->width; x2++) {
            for (u16 y2 = y; y2 < y + image->height; y2++) {
                int pos = ((y2 * image->width) + x2) * 3;
                Draw::pixel(x2, y2, Color(image->rawImage[pos], image->rawImage[pos + 1], image->rawImage[pos + 2], 255));
            }
        }
    }

    u8 Draw::_blend(u32 source, u32 destination, u8 alpha) {
        return (destination * alpha + source * (255 - alpha)) / 255;
    }
}
