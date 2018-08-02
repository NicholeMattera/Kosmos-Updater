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
        SDL_Window * window;
        SDL_Renderer * renderer;

        /* Textures */
        SDL_Texture * a_button;
        SDL_Texture * right_button;
        SDL_Texture * free_controller;
        SDL_Texture * handheld_controller;
        SDL_Texture * left_landscape_controller;
        SDL_Texture * right_landscape_controller;
        SDL_Texture * pro_controller;
        SDL_Texture * downloading;
        SDL_Texture * icon;

        /* Colors */
        SDL_Color background;
        SDL_Color sidebard_background;
        SDL_Color header_footer_divider;
        SDL_Color list_divider;
        SDL_Color active_player_indicator;
        SDL_Color player_indicator;
        SDL_Color selected_background;
        SDL_Color selected_border_1;
        SDL_Color selected_border_2;
        SDL_Color modal_faded_background;
        SDL_Color modal_background;
        SDL_Color text;
        SDL_Color active_text;
        SDL_Color disabled_text;

        /* Fonts */
        PlFontData standardFontData;
        TTF_Font * header_font;
        TTF_Font * footer_font;
        PlFontData extendedFontData;
        TTF_Font * button_font;

        AssetManager();
        ~AssetManager();

        bool initialize();
        void setRenderColor(SDL_Color color);

    private:
        std::string _getFolderName(ColorSetId theme);
        SDL_Texture * _loadAsset(string file);
};