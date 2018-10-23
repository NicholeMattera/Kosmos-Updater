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

#include "PackageSelectScene.hpp"
#include "../SceneDirector.hpp"
#include "../NetManager.hpp"
#include "../ConfigManager.hpp"

PackageSelectScene::PackageSelectScene() {
    _channelOpen = false;
    _bundleOpen = false;
    _focusSelection = 0;

    _headerView = new HeaderView("SDFiles Updater", true);
    _headerView->frame = { 0, 0, 1280, 88 };

    _updateView = new UpdateView("Checking for updates to SDFiles...");
    _updateView->frame.x = 0;
    _updateView->frame.y = 200;

    _statusView = new StatusView("", "");
    _statusView->frame.x = 0;
    _statusView->frame.y = 323;

    _installRowView = new ListRowView("Install Latest SDFiles", "", SUBTITLE);
    _installRowView->frame.x = 215;
    _installRowView->frame.y = 137;
    _installRowView->isLast = true;
    _installRowView->hasFocus = true;

    _settingHeaderView = new ListHeaderView("Settings");
    _settingHeaderView->frame.x = 220;
    _settingHeaderView->frame.y = 208;

    string channel = ConfigManager::getChannel();
    if (channel == "bleeding-edge") {
        _channelSelected = "Bleeding Edge";
    } else {
        _channelSelected = "Stable";
    }

    _channelRowView = new ListRowView("Channel", _channelSelected, VALUE);
    _channelRowView->frame.x = 215;
    _channelRowView->frame.y = 288;

    string bundle = ConfigManager::getBundle();
    if (bundle == "hekate") {
        _bundleSelected = "Hekate";
    } else if (bundle == "hekate-nogc") {
        _bundleSelected = "Hekate (No Gamecard)";
    } else if (bundle == "atmosphere") {
        _bundleSelected = "Atmosphere";
    } else if (bundle == "reinx") {
        _bundleSelected = "ReiNX";
    } else {
        _bundleSelected = "SDFiles";
    }

    _bundleRowView = new ListRowView("Bundle", _bundleSelected, VALUE);
    _bundleRowView->frame.x = 215;
    _bundleRowView->frame.y = 359;
    _bundleRowView->isLast = true;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    vector<string> channelOptions;
    channelOptions.push_back("Stable");
    channelOptions.push_back("Bleeding Edge");

    _channelMultiSelectView = new MultiSelectView("Channel", channelOptions, _channelSelected);

    vector<string> bundleOptions;
    bundleOptions.push_back("SDFiles");
    bundleOptions.push_back("Hekate");
    bundleOptions.push_back("Hekate (No Gamecard)");
    bundleOptions.push_back("Atmosphere");
    bundleOptions.push_back("ReiNX");

    _bundleMultiSelectView = new MultiSelectView("Bundle", bundleOptions, _bundleSelected);

    addSubView(_headerView);
    addSubView(_installRowView);
    addSubView(_settingHeaderView);
    addSubView(_channelRowView);
    addSubView(_bundleRowView);
    addSubView(_footerView);
    addSubView(_channelMultiSelectView);
    addSubView(_bundleMultiSelectView);

    _showUpdateView();
    _versionRequest = NetManager::getLatestSDFilesVersion(channel);
}

PackageSelectScene::~PackageSelectScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_updateView != NULL)
        delete _updateView;

    if (_statusView != NULL)
        delete _statusView;

    if (_installRowView != NULL)
        delete _installRowView;

    if (_settingHeaderView != NULL)
        delete _settingHeaderView;

    if (_channelRowView != NULL)
        delete _channelRowView;

    if (_bundleRowView != NULL)
        delete _bundleRowView;

    if (_footerView != NULL)
        delete _footerView;

    if (_channelMultiSelectView != NULL)
        delete _channelMultiSelectView;

    if (_bundleMultiSelectView != NULL)
        delete _bundleMultiSelectView;

    if (_versionRequest != NULL)
        delete _versionRequest;
}

void PackageSelectScene::handleButton(u32 buttons) {
    if (_channelOpen) {
        _handleButtonsForChannelMutliSelect(buttons);
    } else if (_bundleOpen) {
        _handleButtonsForBundleMutliSelect(buttons);
    } else if (_versionRequest == NULL) {
        if (buttons & KEY_A) {
            Mix_PlayChannel(-1, AssetManager::enter, 0);

            switch(_focusSelection) {
                case 0:
                    SceneDirector::currentScene = SCENE_PACKAGE_DOWNLOAD;
                    break;

                case 1:
                    _focusSelection = -1;
                    _manageFocus();

                    _channelOpen = true;
                    _channelMultiSelectView->hidden = false;
                    break;
                
                case 2:
                    _focusSelection = -1;
                    _manageFocus();

                    _bundleOpen = true;
                    _bundleMultiSelectView->hidden = false;
                    break;
            }
        }

        if (buttons & KEY_B) {
            Mix_PlayChannel(-1, AssetManager::back, 0);
            SceneDirector::exitApp = true;
        }

        if (buttons & KEY_UP && _focusSelection != 0) {
            Mix_PlayChannel(-1, AssetManager::select, 0);
            _focusSelection--;
            _manageFocus();
        }

        if (buttons & KEY_DOWN && _focusSelection != 2) {
            Mix_PlayChannel(-1, AssetManager::select, 0);
            _focusSelection++;
            _manageFocus();
        }
    }
}

void PackageSelectScene::render(SDL_Rect rect, double dTime) {
    if (_versionRequest != NULL) {
        _updateVersionRequest();
    }

    Scene::render(rect, dTime);
}

void PackageSelectScene::_updateVersionRequest() {
    mutexLock(&_versionRequest->mutexRequest);

    _updateView->setProgress(_versionRequest->progress);
    if (_versionRequest->isComplete) {
        _latestVersion = string(_versionRequest->getData());

        _showPackageSelectViews();

        delete _versionRequest;
        _versionRequest = NULL;
    }
    else if (_versionRequest->hasError) {
        _showStatusView(_versionRequest->errorMessage, "Please restart the app to try again.");

        delete _versionRequest;
        _versionRequest = NULL;
    }

    if (_versionRequest != NULL)
        mutexUnlock(&_versionRequest->mutexRequest);
}

void PackageSelectScene::_showUpdateView() {
    _updateView->setProgress(0);
    _updateView->hidden = false;
    _statusView->hidden = true;
    _installRowView->hidden = true;
    _settingHeaderView->hidden = true;
    _channelRowView->hidden = true;
    _bundleRowView->hidden = true;

    for (list<Action *>::iterator it = _footerView->actions.begin(); it != _footerView->actions.end(); it++) {
        delete (*it);
    }
    _footerView->actions.clear();
}

void PackageSelectScene::_showPackageSelectViews() {
    _updateView->hidden = true;
    _statusView->hidden = true;

    _focusSelection = 0;

    _installRowView->hidden = false;
    _installRowView->hasFocus = true;

    string version = ConfigManager::getCurrentVersion();
    if (version.compare(_latestVersion) == 0) {
        _installRowView->setPrimaryText("Reinstall SDFiles");
    } else {
        _installRowView->setPrimaryText("Install Latest SDFiles");
    }

    if (version == "" || version.compare(_latestVersion) == 0) {
        _installRowView->setSecondaryText("Latest Version is " + ((_latestVersion.size() > 7) ? _latestVersion.substr(0, 7) : _latestVersion));
    } else {
        _installRowView->setSecondaryText("You currently have version " + ((version.size() > 7) ? version.substr(0, 7) : version) + " installed, and the latest version is " + ((_latestVersion.size() > 7) ? _latestVersion.substr(0, 7) : _latestVersion + "."));
    }

    _settingHeaderView->hidden = false;
    _settingHeaderView->hasFocus = false;

    _channelRowView->hidden = false;
    _channelRowView->hasFocus = false;

    _bundleRowView->hidden = false;
    _bundleRowView->hasFocus = false;

    for (list<Action *>::iterator it = _footerView->actions.begin(); it != _footerView->actions.end(); it++) {
        delete (*it);
    }
    _footerView->actions.clear();
    _footerView->actions.push_back(new Action(A_BUTTON, "OK"));
    _footerView->actions.push_back(new Action(B_BUTTON, "Quit"));
}

void PackageSelectScene::_showStatusView(string text, string subtext) {
    _statusView->setText(text);
    _statusView->setSubtext(subtext);

    _updateView->hidden = true;
    _statusView->hidden = false;
    _installRowView->hidden = true;
    _settingHeaderView->hidden = true;
    _channelRowView->hidden = true;
    _bundleRowView->hidden = true;

    for (list<Action *>::iterator it = _footerView->actions.begin(); it != _footerView->actions.end(); it++) {
        delete (*it);
    }
    _footerView->actions.clear();
    _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
}

void PackageSelectScene::_manageFocus() {
    _installRowView->hasFocus = false;
    _channelRowView->hasFocus = false;
    _bundleRowView->hasFocus = false;

    switch(_focusSelection) {
        case 0:
            _installRowView->hasFocus = true;
            break;
        
        case 1:
            _channelRowView->hasFocus = true;
            break;

        case 2:
            _bundleRowView->hasFocus = true;
            break;
    }
}

void PackageSelectScene::_handleButtonsForChannelMutliSelect(u32 buttons) {
    if (buttons & KEY_A) {
        Mix_PlayChannel(-1, AssetManager::enter, 0);

        _focusSelection = 1;
        _manageFocus();

        string channel = _channelMultiSelectView->select();
        if (channel == "Bleeding Edge") {
            ConfigManager::setChannel("bleeding-edge");
        } else {
            ConfigManager::setChannel("stable");
        }

        if (channel.compare(_channelSelected) != 0) {
            _resetVersion(true);
        }
        _channelSelected = channel;

        _channelRowView->setSecondaryText(_channelSelected);

        _channelOpen = false;
        _channelMultiSelectView->hidden = true;
        _channelMultiSelectView->reset(_channelSelected);
    }

    if (buttons & KEY_B) {
        Mix_PlayChannel(-1, AssetManager::back, 0);

        _focusSelection = 1;
        _manageFocus();

        _channelOpen = false;
        _channelMultiSelectView->hidden = true;
        _channelMultiSelectView->reset(_channelSelected);
    }

    if (buttons & KEY_UP && _channelMultiSelectView->goUp()) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
    }

    if (buttons & KEY_DOWN && _channelMultiSelectView->goDown()) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
    }
}

void PackageSelectScene::_handleButtonsForBundleMutliSelect(u32 buttons) {
    if (buttons & KEY_A) {
        Mix_PlayChannel(-1, AssetManager::enter, 0);

        _focusSelection = 2;
        _manageFocus();

        string bundle = _bundleMultiSelectView->select();
        if (bundle == "Hekate") {
            ConfigManager::setBundle("hekate");
        } else if (bundle == "Atmosphere") {
            ConfigManager::setBundle("atmosphere");
        } else if (bundle == "Hekate (No Gamecard)") {
            ConfigManager::setBundle("hekate-nogc");
        } else if (bundle == "ReiNX") {
            ConfigManager::setBundle("reinx");
        } else {
            ConfigManager::setBundle("sdfiles");
        }

        if (bundle.compare(_bundleSelected) != 0) {
            _resetVersion(false);
        }
        _bundleSelected = bundle;

        _bundleRowView->setSecondaryText(_bundleSelected);

        _bundleOpen = false;
        _bundleMultiSelectView->hidden = true;
        _bundleMultiSelectView->reset(_bundleSelected);
    }

    if (buttons & KEY_B) {
        Mix_PlayChannel(-1, AssetManager::back, 0);

        _focusSelection = 2;
        _manageFocus();
        
        _bundleOpen = false;
        _bundleMultiSelectView->hidden = true;
        _bundleMultiSelectView->reset(_bundleSelected);
    }

    if (buttons & KEY_UP && _bundleMultiSelectView->goUp()) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
    }

    if (buttons & KEY_DOWN && _bundleMultiSelectView->goDown()) {
        Mix_PlayChannel(-1, AssetManager::select, 0);
    }
}

void PackageSelectScene::_resetVersion(bool channelChange) {
    ConfigManager::setCurrentVersion("");

    if (channelChange) {
        _showUpdateView();

        string channel = ConfigManager::getChannel();
        _versionRequest = NetManager::getLatestSDFilesVersion(channel);
    }
}
