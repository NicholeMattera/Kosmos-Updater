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

#include "View.hpp"

View::View() {
    frame = { 0, 0, 0, 0 };
    hasFocus = false;
}

View::~View() {}

void View::render(SDL_Rect rect) {
    for (list<View *>::iterator it = subviews.begin(); it != subviews.end(); it++) {
        SDL_Rect subviewFrame = (*it)->frame;
        (*it)->render({ rect.x + subviewFrame.x, rect.y + subviewFrame.y, subviewFrame.w, subviewFrame.h });
    }
}

void View::touchStarted() {}
void View::touchMoved() {}
void View::touchEnded() {}

void View::addSubView(View * view) {
    view->superview = view;
    subviews.push_back(view);
}

void View::removeSubView(View * view) {
    view->superview = NULL;
    subviews.remove(view);
}