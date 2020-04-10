// Kosmos Updater
// Copyright (C) 2020 Nichole Mattera
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

#include <jansson.h>

#include "AppUpdateScene.hpp"

#include "../ConfigManager.hpp"
#include "../FileManager.hpp"
#include "../SceneDirector.hpp"

using namespace ku;
using namespace ku::models;
using namespace ku::views;
using namespace std;
using namespace std::placeholders;
using namespace swurl;

namespace ku::scenes {
    AppUpdateScene::AppUpdateScene() {
        SessionManager::onProgressChanged = bind(&AppUpdateScene::_onProgressUpdate, this, _1, _2);
        SessionManager::onCompleted = bind(&AppUpdateScene::_onCompleted, this, _1);
        SessionManager::onError = bind(&AppUpdateScene::_onError, this, _1, _2);

        _headerView = new HeaderView("Kosmos Updater", true);
        _headerView->frame = { 0, 0, 1280, 88 };

        _updateView = new UpdateView("Checking for updates to Kosmos Updater...");
        _updateView->frame.x = 0;
        _updateView->frame.y = 200;

        _statusView = new StatusView("", "");
        _statusView->frame.x = 0;
        _statusView->frame.y = 323;
        _statusView->hidden = true;

        _footerView = new FooterView();
        _footerView->frame = { 0, 647, 1280, 73 };

        addSubView(_headerView);
        addSubView(_updateView);
        addSubView(_statusView);
        addSubView(_footerView);
    }

    AppUpdateScene::~AppUpdateScene() {
        if (_headerView != NULL)
            delete _headerView;

        if (_updateView != NULL)
            delete _updateView;

        if (_statusView != NULL)
            delete _statusView;

        if (_footerView != NULL)
            delete _footerView;

        if (_appVersionRequest != NULL)
            delete _appVersionRequest;

        if (_appRequest != NULL)
            delete _appRequest;
    }

    void AppUpdateScene::handleButton(u32 buttons, double dTime) {
        if (!_statusView->hidden && buttons & KEY_A) {
            SceneDirector::exitApp = true;
        }
    }

    void AppUpdateScene::render(SDL_Rect rect, double dTime) {
        if (_appVersionRequest == NULL) {
            _appVersionRequest = new WebRequest("https://api.github.com/repos/AtlasNX/Kosmos-Updater/releases");
            SessionManager::makeRequest(_appVersionRequest);
        }

        _updateView->setProgress(_downloadProgess);
        Scene::render(rect, dTime);
    }

    void AppUpdateScene::_showStatus(string text, string subtext) {
        _statusView->setText(text);
        _statusView->setSubtext(subtext);

        _updateView->hidden = true;
        _statusView->hidden = false;

        _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
    }

    std::string AppUpdateScene::_sanitizeVersion(std::string version) {
        std::string result = "";

        for (char& c : version) {
            if ((c >= '0' && c <= '9') || c == '.') {
                result += c;
            }
        }

        return result;
    }

    tuple<int, int, int> AppUpdateScene::_parseVersion(string version) {
        size_t pos = 0;
        int index = 0, major = 0, minor = 0, patch = 0;

        while (pos != string::npos) {
            size_t end_pos = version.find(".", pos);

            int versionNumber = 0;
            if (end_pos == string::npos) {
                versionNumber = stoi(version.substr(pos, string::npos));
                pos = string::npos;
            } else {
                versionNumber = stoi(version.substr(pos, end_pos - pos));
                pos = end_pos + 1;
            }

            if (index == 0) {
                major = versionNumber;
            } else if (index == 1) {
                minor = versionNumber;
            } else if (index == 2) {
                patch = versionNumber;
                break;
            }

            index++;
        }

        return make_tuple(major, minor, patch);
    }

    // Swurl Callback Methods

    void AppUpdateScene::_onProgressUpdate(WebRequest * request, double progress) {
        _downloadProgess = progress;
        SceneDirector::currentSceneDirector->render();
    }

    void AppUpdateScene::_onCompleted(WebRequest * request) {
        if (request == _appVersionRequest) {
            json_t * root = json_loads(request->response.rawResponseBody.c_str(), 0, NULL);
            if (!root || !json_is_array(root) || json_array_size(root) < 1) {
                _showStatus("Unable to parse response from GitHub API.", "Please restart the app to try again.");
                return;
            }

            json_t * release = json_array_get(root, 0);
            if (!release || !json_is_object(release)) {
                _showStatus("Unable to parse response from GitHub API.", "Please restart the app to try again.");
                return;
            }

            json_t * tagName = json_object_get(release, "tag_name");
            if (!tagName || !json_is_string(tagName)) {
                _showStatus("Unable to parse response from GitHub API.", "Please restart the app to try again.");
                return;
            }

            auto latestVersion = _parseVersion(_sanitizeVersion(json_string_value(tagName)));

            // No Update
            if (
                VERSION_MAJOR > get<0>(latestVersion) ||
                (VERSION_MAJOR == get<0>(latestVersion) && VERSION_MINOR > get<1>(latestVersion)) ||
                (VERSION_MAJOR == get<0>(latestVersion) && VERSION_MINOR == get<1>(latestVersion) && VERSION_PATCH > get<2>(latestVersion)) ||
                (VERSION_MAJOR == get<0>(latestVersion) && VERSION_MINOR == get<1>(latestVersion) && VERSION_PATCH == get<2>(latestVersion))
            ) {
                json_decref(root);

                SessionManager::onProgressChanged = NULL;
                SessionManager::onCompleted = NULL;
                SessionManager::onError = NULL;

                SceneDirector::currentScene = SCENE_PACKAGE_SELECT;
            }
            // Update
            else {
                json_t * assets = json_object_get(release, "assets");
                if (!assets || !json_is_array(assets) || json_array_size(assets) < 1) {
                    _showStatus("Unable to parse response from GitHub API.", "Please restart the app to try again.");
                    return;
                }

                std::string downloadUrl = "";
                for(size_t i = 0; i < json_array_size(assets); i++) {
                    json_t * asset = json_array_get(assets, i);
                    if (!asset || !json_is_object(asset)) {
                        continue;
                    }

                    json_t * name = json_object_get(asset, "name");
                    if (!name || !json_is_string(name)) {
                        continue;
                    }

                    std::string assetName(json_string_value(name));
                    if (assetName.compare(assetName.length() - 4, 4, ".nro") != 0) {
                        continue;
                    }

                    json_t * browserDownloadUrl = json_object_get(asset, "browser_download_url");
                    if (!browserDownloadUrl || !json_is_string(browserDownloadUrl)) {
                        continue;
                    }

                    downloadUrl = std::string(json_string_value(browserDownloadUrl));
                    break;
                }

                json_decref(root);

                if (downloadUrl.length() == 0) {
                    _showStatus("Unable to find the latest release assets.", "Please restart the app to try again.");
                    return;
                }

                _updateView->setProgress(0);
                _updateView->setText("Getting the latest version of Kosmos Updater...");

                _appRequest = new WebRequest(downloadUrl);
                SessionManager::makeRequest(_appRequest);
            }
        } else if (request == _appRequest) {
            romfsExit();
            FileManager::writeFile("KosmosUpdater.nro", request->response.rawResponseBody);
            _showStatus("Kosmos Updater has been updated to version " + _appVersionRequest->response.rawResponseBody + "!", "Please restart the app.");
        }
    }

    void AppUpdateScene::_onError(WebRequest * request, string error) {
        _showStatus(error, "Please restart the app to try again.");
    }
}
