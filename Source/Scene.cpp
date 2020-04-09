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

#include "Scene.hpp"

namespace KosmosUpdater {
    void Scene::render(Rect rect, double dTime) {
        for (auto const& view : subviews) {
            if (auto viewTemp = view.lock()) {
                if (viewTemp->isHidden)
                    continue;

                Rect subviewFrame = viewTemp->frame;
                // Models::ViewRender subViewRender = view->render({ rect.x + subviewFrame.x, rect.y + subviewFrame.y, subviewFrame.w, subviewFrame.h }, dTime);
                viewTemp->render(
                    Rect(
                        (u16) (rect.x + subviewFrame.x),
                        (u16) (rect.y + subviewFrame.y),
                        subviewFrame.width,
                        subviewFrame.height
                    ),
                    dTime
                );
                // SDL_RenderCopy(Application::renderer, subViewRender.texture, NULL, &subViewRender.bounds);
            }
        }
    }

    void Scene::addSubView(std::shared_ptr<View> view) {
        subviews.push_back(view);
    }
}
