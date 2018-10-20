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

#include <SDL2/SDL2_gfxPrimitives.h>
#include "ListRowView.hpp"
#include "../SceneDirector.hpp"

ListRowView::ListRowView(string primaryText, string secondaryText, ListRowStyle style) : View() {
    isLast = false;
    hasCheckmark = false;
    frame = { 0, 0, 850, 71 };

    _timeElapsed = 0;
    _style = style;

    _primaryTextView = new TextView(AssetManager::body_font, primaryText, AssetManager::text);
    _primaryTextView->textAlignment = LEFT_ALIGN;
    if (style != SUBTITLE) {
        _primaryTextView->frame = { 21, 26, 808, 0 };
    } else {
        _primaryTextView->frame = { 21, 10, 808, 0 };
    }
    addSubView(_primaryTextView);

    if (style != DEFAULT) {
        _secondaryTextView = new TextView(AssetManager::subbody_font, secondaryText, (style == SUBTITLE) ? AssetManager::disabled_text : AssetManager::active_text);
        if (style != SUBTITLE) {
            _secondaryTextView->textAlignment = RIGHT_ALIGN;
            _secondaryTextView->frame = { 21, 29, 808, 0 };
        } else {
            _secondaryTextView->textAlignment = LEFT_ALIGN;
            _secondaryTextView->frame = { 21, 41, 808, 0 };
        }

        addSubView(_secondaryTextView);
    } else {
        _secondaryTextView = NULL;
    }

    if (AssetManager::checkmark == NULL) {
        SDL_Surface *surface = TTF_RenderGlyph_Blended(AssetManager::button_font, 0xE14B, AssetManager::background);
        AssetManager::checkmark = SDL_CreateTextureFromSurface(SceneDirector::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

ListRowView::~ListRowView() {
    if (_primaryTextView != NULL)
        delete _primaryTextView;

    if (_secondaryTextView != NULL)
        delete _secondaryTextView;
}

void ListRowView::render(SDL_Rect rect, double dTime) {
    // Draw Separators
    AssetManager::setRenderColor(AssetManager::list_divider);
    SDL_RenderDrawLine(SceneDirector::renderer, rect.x + 5, rect.y, rect.x + rect.w - 10, rect.y);
    if (isLast) {
        SDL_RenderDrawLine(SceneDirector::renderer, rect.x + 5, rect.y + rect.h, rect.x + rect.w - 10, rect.y + rect.h);
    }

    // Draw Focus Background
    if (hasFocus) {
        _timeElapsed += dTime;

        SDL_Rect backgroundFrame = { rect.x + 5, rect.y + 5, rect.w - 10, rect.h - 10 };
        AssetManager::setRenderColor(AssetManager::selected_background);
        SDL_RenderFillRect(SceneDirector::renderer, &backgroundFrame);

        SDL_Color selectionColor = _generateSelectionColor();
        _drawBorders(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, selectionColor);
    } else {
        _timeElapsed = 0;
    }

    if (hasCheckmark) {
        SDL_Color activeColor = AssetManager::active_text;
        filledCircleRGBA(SceneDirector::renderer, rect.x + rect.w - 35, rect.y + 36, 15, activeColor.r, activeColor.g, activeColor.b, activeColor.a);
        aacircleRGBA(SceneDirector::renderer, rect.x + rect.w - 35, rect.y + 36, 15, activeColor.r, activeColor.g, activeColor.b, activeColor.a);

        SDL_Rect checkmarkFrame = { rect.x + rect.w - 47, rect.y + 24, 25, 25 };
        SDL_RenderCopy(SceneDirector::renderer, AssetManager::checkmark, NULL, &checkmarkFrame);
    }

    // Render any subviews.
    View::render(rect, dTime);
}

void ListRowView::setPrimaryText(string text) {
    _primaryTextView->setText(text);
}

void ListRowView::setSecondaryText(string text) {
    _secondaryTextView->setText(text);
}

SDL_Color ListRowView::_generateSelectionColor() {
    SDL_Color color1 = AssetManager::selected_border_1;
    SDL_Color color2 = AssetManager::selected_border_2;
    SDL_Color result = { 0, 0, 0, 255 };

    
    if (_timeElapsed > 2000)
        _timeElapsed -= 2000;

    // Color1 -> Color2
    if (floor(((int) _timeElapsed) / 1000) == 0) {
        double time = (_timeElapsed / 1000) * -1;
        result.r = color1.r + floor((color1.r - color2.r) * time);
        result.g = color1.g + floor((color1.g - color2.g) * time);
        result.b = color1.b + floor((color1.b - color2.b) * time);
    }
    // Color2 -> Color1
    else {
        double time = ((_timeElapsed - 1000) / 1000) * -1;
        result.r = color2.r + floor((color2.r - color1.r) * time);
        result.g = color2.g + floor((color2.g - color1.g) * time);
        result.b = color2.b + floor((color2.b - color1.b) * time);
    }

    return result;
}

void ListRowView::_drawBorders(int x1, int y1, int x2, int y2, SDL_Color color) {
    // Top
    thickLineRGBA(
            SceneDirector::renderer,
            x1, y1 + 3, x2, y1 + 3,
            5,
            color.r, color.g, color.b, color.a);

    // Right
    thickLineRGBA(
            SceneDirector::renderer,
            x2 - 2, y1 + 3, x2 - 2, y2 - 3,
            5,
            color.r, color.g, color.b, color.a);

    // Bottom
    thickLineRGBA(
            SceneDirector::renderer,
            x2, y2 - 3, x1, y2 - 3,
            5,
            color.r, color.g, color.b, color.a);

    // Left
    thickLineRGBA(
            SceneDirector::renderer,
            x1 + 2, y2 - 3, x1 + 2, y1 + 3,
            5,
            color.r, color.g, color.b, color.a);
}
