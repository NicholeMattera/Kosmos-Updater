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

#include <cstring>

#include "Application.hpp"
#include "Managers/Theme.hpp"

namespace KosmosUpdater {
    Application::Application() {
        this->_window = nwindowGetDefault();
        framebufferCreate(
            &this->_framebuffer,
            this->_window,
            1280,
            720,
            PIXEL_FORMAT_RGBA_8888,
            2);
        framebufferMakeLinear(&this->_framebuffer);

        Managers::Theme::Instance()->updateTheme();
    }

    Application::~Application() {
        framebufferClose(&this->_framebuffer);
    }

    int Application::start(std::shared_ptr<Scene> scene) {
        Application::_CurrentScene = scene;

        // Main Game Loop
        while (appletMainLoop())
        {
            auto dTime = _getDeltaTime();

            // Handle controller inputs
            hidScanInput();
            this->_processControllerForScene(CONTROLLER_P1_AUTO, dTime);

            Application::CurrentFramebuffer = (u8 *) framebufferBegin(&this->_framebuffer, &Application::Stride);
            memset(Application::CurrentFramebuffer, 0, Application::Stride * 720);

            if (Application::_CurrentScene) {
                this->_render(dTime);
                Application::_CurrentScene->tick(Rect(0, 0, 1280, 720), dTime);
            } else {
                break;
            }

            // Present our frame.
            framebufferEnd(&this->_framebuffer);
        }

        return 0;
    }

    void Application::render() {
        if (Application::_CurrentScene) {
            auto dTime = _getDeltaTime();
            _render(dTime);

            // Present our frame.
            framebufferEnd(&this->_framebuffer);
        }
    }

    void Application::SwitchScene(std::shared_ptr<Scene> scene) {
        Application::_CurrentScene = scene;
    }

    double Application::_getDeltaTime() {
        _lastTime = _nowTime;
        _nowTime = svcGetSystemTick();
        return _nowTime - _lastTime / 19200;
    }

    void Application::_processControllerForScene(HidControllerID id, double dTime) {
        if (Application::_CurrentScene)
            Application::_CurrentScene->buttonsHeld(hidKeysHeld(id), dTime);

        if (Application::_CurrentScene)
            Application::_CurrentScene->buttonsDown(hidKeysDown(id), dTime);

        if (Application::_CurrentScene)
            Application::_CurrentScene->buttonsUp(hidKeysUp(id), dTime);
    }

    void Application::_render(double dTime) {
        Draw::fill(Rect(0, 0, 1280, 720), Application::_CurrentScene->background);

        // Call rendering routines.
        Application::_CurrentScene->render(Rect(0, 0, 1280, 720), dTime);
    }
}
