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

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>

using namespace std;

class AssetManager {
    public:
        /* Textures */
        static SDL_Texture * a_button;
        static SDL_Texture * b_button;
        static SDL_Texture * x_button;
        static SDL_Texture * y_button;
        static SDL_Texture * downloading;
        static SDL_Texture * icon;

        /* Colors */
        static ColorSetId theme;
        static SDL_Color background;
        static SDL_Color sidebard_background;
        static SDL_Color header_footer_divider;
        static SDL_Color list_divider;
        static SDL_Color active_player_indicator;
        static SDL_Color player_indicator;
        static SDL_Color selected_background;
        static SDL_Color selected_border_1;
        static SDL_Color selected_border_2;
        static SDL_Color modal_faded_background;
        static SDL_Color modal_background;
        static SDL_Color text;
        static SDL_Color active_text;
        static SDL_Color disabled_text;

        /* Fonts */
        static PlFontData standardFontData;
        static TTF_Font * header_font;
        static TTF_Font * body_font;
        static TTF_Font * subbody_font;
        static PlFontData extendedFontData;
        static TTF_Font * button_font;

        static bool initialize();
        static void dealloc();
        static void setRenderColor(SDL_Color color);
        static SDL_Texture * loadAsset(string file);
};
