// Kosmos Updater
// Copyright (C) 2019 Steven Mattera
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

#include "PackageDownloadScene.hpp"
#include "../SceneDirector.hpp"
#include "../NetManager.hpp"
#include "../ConfigManager.hpp"
#include "../FileManager.hpp"

using namespace std::placeholders;

PackageDownloadScene::PackageDownloadScene() {
    bpcInitialize();

    _shouldApplyESPatches = (FileManager::esPatchesExists() || ConfigManager::getForceESPatches());
    
    _headerView = new HeaderView("Kosmos Updater", true);
    _headerView->frame = { 0, 0, 1280, 88 };

    _updateView = new UpdateView("Downloading the latest Kosmos...");
    _updateView->frame.x = 0;
    _updateView->frame.y = 200;

    _statusView = new StatusView("", "");
    _statusView->frame.x = 0;
    _statusView->frame.y = 323;
    _statusView->hidden = true;

    _footerView = new FooterView();
    _footerView->frame = { 0, 647, 1280, 73 };

    vector<string> buttons;
    buttons.push_back("Restart");
    buttons.push_back("Quit");
    _restartAlertView = new AlertView("Are you sure?", "Restarting while using an ExFAT formatted SD card\nwill cause it to corrupt. It is recommended that if\nyou have an ExFAT formatted SD card to quit,\npress the home button, and restart the switch.", buttons);
    _restartAlertView->onDismiss = bind(&PackageDownloadScene::_onAlertViewDismiss, this, _1, _2);

    addSubView(_headerView);
    addSubView(_updateView);
    addSubView(_statusView);
    addSubView(_footerView);

    string bundle = ConfigManager::getBundle();
    string channel = ConfigManager::getChannel();
    
    _packageRequest = NetManager::getLatestKosmos(bundle, channel);
    _packageDelete = NULL;
    _packageExtract = NULL;
    _esPatchesRequest = NULL;
    _esPatchesExtract = NULL;
    _packageDisableGC = NULL;
}

PackageDownloadScene::~PackageDownloadScene() {
    if (_headerView != NULL)
        delete _headerView;

    if (_updateView != NULL)
        delete _updateView;

    if (_statusView != NULL)
        delete _statusView;

    if (_footerView != NULL)
        delete _footerView;

    if (_packageRequest != NULL)
        delete _packageRequest;

    if (_packageDelete != NULL)
        delete _packageDelete;

    if (_packageExtract != NULL)
        delete _packageExtract;

    if (_esPatchesRequest != NULL)
        delete _esPatchesRequest;

    if (_esPatchesExtract != NULL)
        delete _esPatchesExtract;

    if (_packageDisableGC != NULL)
        delete _packageDisableGC;

    bpcExit();
}

void PackageDownloadScene::handleButton(u32 buttons, double dTime) {
    if (!_statusView->hidden && buttons & KEY_A) {
        SceneDirector::exitApp = true;
    }

    if (!_statusView->hidden && _footerView->actions.size() == 2 && buttons & KEY_X) {
        _restartAlertView->reset();
        _restartAlertView->show();
    }
}

void PackageDownloadScene::render(SDL_Rect rect, double dTime) {
    if (_packageRequest != NULL) {
        _updatePackageRequest();
    }
    else if (_packageDelete != NULL) {
        _updatePackageDelete();
    }
    else if (_packageExtract != NULL) {
        _updatePackageExtract();
    }
    else if (_esPatchesRequest != NULL) {
        _updateEsPatchesRequest();
    }
    else if (_esPatchesExtract != NULL) {
        _updateEsPatchesExtract();
    }
    else if (_packageDisableGC != NULL) {
        _updatePackageDisableGC();
    }

    Scene::render(rect, dTime);
}

void PackageDownloadScene::_updatePackageRequest() {
    mutexLock(&_packageRequest->mutexRequest);

    _updateView->setProgress(_packageRequest->progress);
    if (_packageRequest->isComplete) {
        FileManager::writeFile("temp.zip", _packageRequest);
        _versionNumber = _packageRequest->getVersionNumber();
        _numberOfFiles = stoi(_packageRequest->getNumberOfFiles());

        delete _packageRequest;
        _packageRequest = NULL;

        _updateView->setText("Removing old package...");
        _updateView->setProgress(0);

        _packageDelete = new ThreadObj();
        FileManager::cleanUpFiles(_packageDelete);
    }
    else if (_packageRequest->hasError) {
        _showStatus(_packageRequest->errorMessage, "Please restart the app to try again.", false);

        delete _packageRequest;
        _packageRequest = NULL;
    }

    if (_packageRequest != NULL)
        mutexUnlock(&_packageRequest->mutexRequest);
}

void PackageDownloadScene::_updatePackageDelete() {
    mutexLock(&_packageDelete->mutexRequest);

    _updateView->setProgress(_packageDelete->progress);
    if (_packageDelete->isComplete) {
        delete _packageDelete;
        _packageDelete = NULL;

        _updateView->setText("Extracting the latest package...");
        _updateView->setProgress(0);

        _packageExtract = new Zip("temp.zip", "sdmc:/", _numberOfFiles);
        FileManager::extract(_packageExtract);
    }

    if (_packageDelete != NULL)
        mutexUnlock(&_packageDelete->mutexRequest);
}

void PackageDownloadScene::_updatePackageExtract() {
    mutexLock(&_packageExtract->mutexRequest);

    _updateView->setProgress(_packageExtract->progress);
    if (_packageExtract->isComplete) {
        delete _packageExtract;
        _packageExtract = NULL;

        FileManager::deleteFile("temp.zip");
        ConfigManager::setCurrentVersion(_versionNumber);

        if (_shouldApplyESPatches) {
            _updateView->setText("Downloading the latest ES Patches...");
            _updateView->setProgress(0);

            _esPatchesRequest = NetManager::getLatestKosmos("es_patches", "stable");
        } else {
            _updateView->setText("Applying disabled game cart option...");
            _updateView->setProgress(0);

            _packageDisableGC = new ThreadObj();
            FileManager::applyNoGC(_packageDisableGC);
        }
    }
    else if (_packageExtract->hasError) {
        delete _packageExtract;
        _packageExtract = NULL;

        FileManager::deleteFile("temp.zip");

        _showStatus(_packageExtract->errorMessage, "Please restart the app to try again.", false);
    }

    if (_packageExtract != NULL)
        mutexUnlock(&_packageExtract->mutexRequest);
}

void PackageDownloadScene::_updateEsPatchesRequest() {
    mutexLock(&_esPatchesRequest->mutexRequest);

    _updateView->setProgress(_esPatchesRequest->progress);
    if (_esPatchesRequest->isComplete) {
        FileManager::writeFile("temp.zip", _esPatchesRequest);
        int numberOfFiles = stoi(_esPatchesRequest->getNumberOfFiles());

        delete _esPatchesRequest;
        _esPatchesRequest = NULL;

       _updateView->setText("Extracting the latest ES Patches...");
        _updateView->setProgress(0);

        _esPatchesExtract = new Zip("temp.zip", "sdmc:/", numberOfFiles);
        FileManager::extract(_esPatchesExtract);
    }
    else if (_esPatchesRequest->hasError) {
        _showStatus(_esPatchesRequest->errorMessage, "Please restart the app to try again.", false);

        delete _esPatchesRequest;
        _esPatchesRequest = NULL;
    }

    if (_esPatchesRequest != NULL)
        mutexUnlock(&_esPatchesRequest->mutexRequest);
}

void PackageDownloadScene::_updateEsPatchesExtract() {
    mutexLock(&_esPatchesExtract->mutexRequest);

    _updateView->setProgress(_esPatchesExtract->progress);
    if (_esPatchesExtract->isComplete) {
        delete _esPatchesExtract;
        _esPatchesExtract = NULL;

        FileManager::deleteFile("temp.zip");

        _updateView->setText("Applying disabled game cart option...");
        _updateView->setProgress(0);

        _packageDisableGC = new ThreadObj();
        FileManager::applyNoGC(_packageDisableGC);
    }
    else if (_esPatchesExtract->hasError) {
        delete _esPatchesExtract;
        _esPatchesExtract = NULL;

        FileManager::deleteFile("temp.zip");

        _showStatus(_esPatchesExtract->errorMessage, "Please restart the app to try again.", false);
    }

    if (_esPatchesExtract != NULL)
        mutexUnlock(&_esPatchesExtract->mutexRequest);
}

void PackageDownloadScene::_updatePackageDisableGC() {
    mutexLock(&_packageDisableGC->mutexRequest);

    _updateView->setProgress(_packageDisableGC->progress);
    if (_packageDisableGC->isComplete) {
        delete _packageDisableGC;
        _packageDisableGC = NULL;

        FileManager::deleteFile("temp.zip");
        ConfigManager::setCurrentVersion(_versionNumber);

        _showStatus("SD Files has been updated to version " + _getVersionNumber(_versionNumber) + "!", "Please restart your Switch to run the latest SD Files.", true);
    }

    if (_packageDisableGC != NULL)
        mutexUnlock(&_packageDisableGC->mutexRequest);
}

void PackageDownloadScene::_showStatus(string text, string subtext, bool wasSuccessful) {
    _statusView->setText(text);
    _statusView->setSubtext(subtext);

    _updateView->hidden = true;
    _statusView->hidden = false;

    _footerView->actions.push_back(new Action(A_BUTTON, "Quit"));
    if (wasSuccessful) {
        _footerView->actions.push_back(new Action(X_BUTTON, "Restart"));
    }
}

void PackageDownloadScene::_onAlertViewDismiss(ModalView * view, bool success) {
    if (success) {
        if (_restartAlertView->getSelectedOption() == 0) {
            bpcRebootSystem();
        }
        else {
            SceneDirector::exitApp = true;
        }
    }
}

string PackageDownloadScene::_getVersionNumber(string version) {
    return (version.size() > 7 && ConfigManager::getChannel() != "stable") ? version.substr(0, 7) : version;
}
