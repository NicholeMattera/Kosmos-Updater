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

#include "MultiSelectView.hpp"
#include "../AssetManager.hpp"
#include "../SceneDirector.hpp"

MultiSelectView::MultiSelectView(string title, vector<string> options, string selectedOption) {
    _startY = 720 - (224 + min((int) options.size(), 4) * 71);
    _focusSelection = 0;
    _options = options;
    hidden = true;

    for (vector<string>::iterator it = options.begin(); it != options.end(); it++) {
        string option = *it;

        ListRowView * rowView = new ListRowView(option, "", DEFAULT);
        rowView->frame.x = 215;
        rowView->frame.y = _startY + 120 + (_listRowViews.size() * 71);
        rowView->hasFocus = _listRowViews.size() == 0;
        rowView->hasCheckmark = option.compare(selectedOption) == 0;

        _listRowViews.push_back(rowView);
        addSubView(rowView);
    }
    _listRowViews.back()->isLast = true;

    _headerView = new HeaderView(title.c_str(), false);
    _headerView->frame = { 0, _startY, 1280, 88 };
    addSubView(_headerView);

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };
    _footerView->actions.push_back(new Action(A_BUTTON, "OK"));
    _footerView->actions.push_back(new Action(B_BUTTON, "Back"));
    addSubView(_footerView);
}

MultiSelectView::~MultiSelectView() {
    if (_headerView != NULL)
        delete _headerView;

    for (vector<ListRowView *>::iterator it = _listRowViews.begin(); it != _listRowViews.end(); it++) {
        ListRowView * rowView = *it;
        
        if (rowView != NULL)
            delete rowView;
    }
    _listRowViews.clear();

    if (_footerView != NULL)
        delete _footerView;
}

void MultiSelectView::render(SDL_Rect rect, double dTime) {
    // Draw Faded background.
    AssetManager::setRenderColor(AssetManager::modal_faded_background);
    SDL_Rect fadedBGFrame = { 0, 0, 1280, 720 };
    SDL_SetRenderDrawBlendMode(SceneDirector::renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(SceneDirector::renderer, &fadedBGFrame);

    // Draw background.
    AssetManager::setRenderColor(AssetManager::background);
    SDL_Rect bgFrame = { 0, _startY, 1280, 720 - _startY };
    SDL_RenderFillRect(SceneDirector::renderer, &bgFrame);

    // Render any subviews.
    View::render(rect, dTime);
}

bool MultiSelectView::goUp() {
    if (_focusSelection != 0) {
        _focusSelection--;

        int index = 0;
        for (vector<ListRowView *>::iterator it = _listRowViews.begin(); it != _listRowViews.end(); it++) {
            (*it)->hasFocus = false;

            if (_focusSelection >= 3) {
                (*it)->frame.y = (_startY + 120 + index * 71) - ((_focusSelection - 3) * 71);
            }

            index++;
        }

        _listRowViews[_focusSelection]->hasFocus = true;
        return true;
    }

    return false;
}

bool MultiSelectView::goDown() {
    if (_focusSelection != _listRowViews.size() -1) {
        _focusSelection++;
        
        int index = 0;
        for (vector<ListRowView *>::iterator it = _listRowViews.begin(); it != _listRowViews.end(); it++) {
            (*it)->hasFocus = false;

            if (_focusSelection >= 3) {
                (*it)->frame.y = (_startY + 120 + index * 71) - ((_focusSelection - 3) * 71);
            }

            index++;
        }

        _listRowViews[_focusSelection]->hasFocus = true;

        return true;
    }

    return false;
}

string MultiSelectView::select() {
    return _options[_focusSelection];
}

void MultiSelectView::reset(string selectedOption) {
    _focusSelection = 0;

    for (long unsigned int i = 0; i < _listRowViews.size(); i++) {
        _listRowViews[i]->hasFocus = (i == 0);
        _listRowViews[i]->hasCheckmark = _options[i].compare(selectedOption) == 0;
    }
}
