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

AssetManager::AssetManager() {
    window = NULL;
    renderer = NULL;
    a_button = NULL;
    right_button = NULL;
    free_controller = NULL;
    handheld_controller = NULL;
    left_landscape_controller = NULL;
    right_landscape_controller = NULL;
    pro_controller = NULL;
    downloading = NULL;
    icon = NULL;
}

AssetManager::~AssetManager() {
    if (downloading != NULL) 
        SDL_DestroyTexture(downloading);

    if (icon != NULL)
        SDL_DestroyTexture(icon);

    if (header_font != NULL)
        TTF_CloseFont(header_font);

    if (footer_font != NULL)
        TTF_CloseFont(footer_font);

    if (button_font != NULL)
        TTF_CloseFont(button_font);

    TTF_Quit();

    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);

    if (window != NULL)
        SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    plExit();
    setsysExit();
    romfsExit();
};

bool AssetManager::initialize() {
    Result rc;

    romfsInit();
    setsysInitialize();
    plInitialize();

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        return false;

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
    if (!window)
        return false;

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
    if (!renderer)
        return false;

    TTF_Init();

    ColorSetId theme;
    setsysGetColorSetId(&theme);
    string folderName = _getFolderName(theme);

    downloading = _loadAsset("romfs:/" + folderName + "/downloading.png");
    icon = _loadAsset("romfs:/" + folderName + "/icon.png");

    if (theme == ColorSetId_Light) {
        background = { 235, 235, 235, 255 };
        sidebard_background = { 240, 240, 240, 255 };
        header_footer_divider = { 45, 45, 45, 255 };
        list_divider = { 205, 205, 205, 255 };
        active_player_indicator = { 158, 228, 0, 255 };
        player_indicator = { 125, 125, 125, 255 };
        selected_background = { 253, 253, 253, 255 };
        selected_border_1 = { 0, 255, 196, 255 };
        selected_border_2 = { 22, 146, 197, 255 };
        modal_faded_background = { 18, 27, 36, 229 };
        modal_background = { 240, 240, 240, 255 };
        text = { 45, 45, 45, 255 };
        active_text = { 50, 80, 240, 255 };
        disabled_text = { 165, 165, 165, 255 };
    } else {
        background = { 45, 45, 45, 255 };
        sidebard_background = { 51, 51, 51, 255 };
        header_footer_divider = { 255, 255, 255, 255 };
        list_divider = { 77, 77, 77, 255 };
        active_player_indicator = { 158, 228, 0, 255 };
        player_indicator = { 125, 125, 125, 255 };
        selected_background = { 34, 34, 34, 255 };
        selected_border_1 = { 0, 255, 196, 255 };
        selected_border_2 = { 22, 146, 197, 255 };
        modal_faded_background = { 18, 27, 36, 229 };
        modal_background = { 70, 70, 70, 255 };
        text = { 255, 255, 255, 255 };
        active_text = { 0, 255, 196, 255 };
        disabled_text = { 125, 125, 125, 255 };
    }

    rc = plGetSharedFontByType(&standardFontData, PlSharedFontType_Standard);
    if (R_FAILED(rc)) {
        return false;
    }

    header_font = TTF_OpenFontRW(SDL_RWFromMem(standardFontData.address, standardFontData.size), 1, 28);
    footer_font = TTF_OpenFontRW(SDL_RWFromMem(standardFontData.address, standardFontData.size), 1, 23);

    if (!header_font || !footer_font)
        return false;

    rc = plGetSharedFontByType(&extendedFontData, PlSharedFontType_NintendoExt);
    if (R_FAILED(rc)) {
        return false;
    }
    
    button_font = TTF_OpenFontRW(SDL_RWFromMem(extendedFontData.address, extendedFontData.size), 1, 25);

    if (!button_font)
        return false;

    return true;
};

void AssetManager::setRenderColor(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

string AssetManager::_getFolderName(ColorSetId theme) {
    return (theme == ColorSetId_Light) ? "light" : "dark";
}

SDL_Texture * AssetManager::_loadAsset(string file) {
    SDL_Surface * image = IMG_Load(file.c_str());
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
};