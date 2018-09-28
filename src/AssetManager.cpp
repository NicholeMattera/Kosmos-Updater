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

#include "AssetManager.hpp"
#include "SceneDirector.hpp"

/* Textures */
SDL_Texture * AssetManager::a_button = NULL;
SDL_Texture * AssetManager::b_button = NULL;
SDL_Texture * AssetManager::x_button = NULL;
SDL_Texture * AssetManager::y_button = NULL;
SDL_Texture * AssetManager::downloading = NULL;
SDL_Texture * AssetManager::icon = NULL;

/* Colors */
ColorSetId AssetManager::theme;
SDL_Color AssetManager::background;
SDL_Color AssetManager::sidebard_background;
SDL_Color AssetManager::header_footer_divider;
SDL_Color AssetManager::list_divider;
SDL_Color AssetManager::active_player_indicator;
SDL_Color AssetManager::player_indicator;
SDL_Color AssetManager::selected_background;
SDL_Color AssetManager::selected_border_1;
SDL_Color AssetManager::selected_border_2;
SDL_Color AssetManager::modal_faded_background;
SDL_Color AssetManager::modal_background;
SDL_Color AssetManager::text;
SDL_Color AssetManager::active_text;
SDL_Color AssetManager::disabled_text;

/* Fonts */
PlFontData AssetManager::standardFontData;
TTF_Font * AssetManager::header_font = NULL;
TTF_Font * AssetManager::body_font = NULL;
PlFontData AssetManager::extendedFontData;
TTF_Font * AssetManager::button_font = NULL;

void AssetManager::dealloc() {
    if (AssetManager::button_font != NULL)
        TTF_CloseFont(AssetManager::button_font);

    if (AssetManager::body_font != NULL)
        TTF_CloseFont(AssetManager::body_font);

    if (AssetManager::header_font != NULL)
        TTF_CloseFont(AssetManager::header_font);

    if (AssetManager::icon != NULL)
        SDL_DestroyTexture(AssetManager::icon);
    
    if (AssetManager::downloading != NULL) 
        SDL_DestroyTexture(AssetManager::downloading);

    if (AssetManager::y_button != NULL)
        SDL_DestroyTexture(AssetManager::y_button);

    if (AssetManager::x_button != NULL)
        SDL_DestroyTexture(AssetManager::x_button);

    if (AssetManager::b_button != NULL)
        SDL_DestroyTexture(AssetManager::b_button);

    if (AssetManager::a_button != NULL) 
        SDL_DestroyTexture(AssetManager::a_button);
};

bool AssetManager::initialize() {
    Result rc;

    setsysGetColorSetId(&AssetManager::theme);
    if (AssetManager::theme == ColorSetId_Light) {
        AssetManager::background = { 235, 235, 235, 255 };
        AssetManager::sidebard_background = { 240, 240, 240, 255 };
        AssetManager::header_footer_divider = { 45, 45, 45, 255 };
        AssetManager::list_divider = { 205, 205, 205, 255 };
        AssetManager::active_player_indicator = { 158, 228, 0, 255 };
        AssetManager::player_indicator = { 125, 125, 125, 255 };
        AssetManager::selected_background = { 253, 253, 253, 255 };
        AssetManager::selected_border_1 = { 0, 255, 196, 255 };
        AssetManager::selected_border_2 = { 22, 146, 197, 255 };
        AssetManager::modal_faded_background = { 18, 27, 36, 229 };
        AssetManager::modal_background = { 240, 240, 240, 255 };
        AssetManager::text = { 45, 45, 45, 255 };
        AssetManager::active_text = { 50, 80, 240, 255 };
        AssetManager::disabled_text = { 165, 165, 165, 255 };
    } else {
        AssetManager::background = { 45, 45, 45, 255 };
        AssetManager::sidebard_background = { 51, 51, 51, 255 };
        AssetManager::header_footer_divider = { 255, 255, 255, 255 };
        AssetManager::list_divider = { 77, 77, 77, 255 };
        AssetManager::active_player_indicator = { 158, 228, 0, 255 };
        AssetManager::player_indicator = { 125, 125, 125, 255 };
        AssetManager::selected_background = { 34, 34, 34, 255 };
        AssetManager::selected_border_1 = { 0, 255, 196, 255 };
        AssetManager::selected_border_2 = { 22, 146, 197, 255 };
        AssetManager::modal_faded_background = { 18, 27, 36, 229 };
        AssetManager::modal_background = { 70, 70, 70, 255 };
        AssetManager::text = { 255, 255, 255, 255 };
        AssetManager::active_text = { 0, 255, 196, 255 };
        AssetManager::disabled_text = { 125, 125, 125, 255 };
    }

    rc = plGetSharedFontByType(&AssetManager::standardFontData, PlSharedFontType_Standard);
    if (R_FAILED(rc))
        return false;

    AssetManager::header_font = TTF_OpenFontRW(SDL_RWFromMem(AssetManager::standardFontData.address, AssetManager::standardFontData.size), 1, 28);
    AssetManager::body_font = TTF_OpenFontRW(SDL_RWFromMem(AssetManager::standardFontData.address, AssetManager::standardFontData.size), 1, 23);
    if (!AssetManager::header_font || !AssetManager::body_font)
        return false;

    rc = plGetSharedFontByType(&AssetManager::extendedFontData, PlSharedFontType_NintendoExt);
    if (R_FAILED(rc))
        return false;
    
    AssetManager::button_font = TTF_OpenFontRW(SDL_RWFromMem(AssetManager::extendedFontData.address, AssetManager::extendedFontData.size), 1, 25);
    if (!AssetManager::button_font)
        return false;

    return true;
};

void AssetManager::setRenderColor(SDL_Color color) {
    SDL_SetRenderDrawColor(SceneDirector::renderer, color.r, color.g, color.b, color.a);
}

SDL_Texture * AssetManager::loadAsset(string file) {
    string themeDirectory = (AssetManager::theme == ColorSetId_Light) ? "light" : "dark";
    string path = "romfs:/" + themeDirectory + "/" + file;
    SDL_Surface * image = IMG_Load(path.c_str());
    SDL_Texture * texture = SDL_CreateTextureFromSurface(SceneDirector::renderer, image);
    SDL_FreeSurface(image);
    return texture;
};