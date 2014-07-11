 /**
  * Copyright (C) 2014, Jaguar Land Rover
  *
  * Author: Jonatan Palsson <jonatan.palsson@pelagicore.com>
  *
  * This file is part of the GENIVI Media Manager Proof-of-Concept
  * For further information, see http://genivi.org/
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
  */

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include <gio/gio.h>
#include <CommonAPI/CommonAPI.h>

#include "lms.h"
#include "browserprovider.h"
#include "playerprovider.h"
#include "indexerstub.h"
#include "browserstub.h"
#include "playerstub.h"

template<typename StubImpl, typename Provider>
bool registerService (std::shared_ptr<CommonAPI::Runtime> runtime,
                     Provider *provider,
                      std::string address,
                      std::string friendly,
                      MmError *e) {
        auto factory = runtime->createFactory();
        if (!factory) {
            std::cerr << "Error: Unable to create factory!\n";
            return false;
        }

        auto servicePublisher = runtime->getServicePublisher();
        if (!servicePublisher) {
            std::cerr << "Error: Unable to load service publisher!\n";
            return false;
        }

        if (!e) {
            auto stub = std::make_shared<StubImpl>(provider);
            const bool success= servicePublisher->registerService(stub,
                                                                   address,
                                                                   factory);
            if (!success) {
                std::cerr << "Error: Unable to register " << friendly << " service!\n";
            }
        } else {
            std::cout << "Error connecting to " << friendly << ": " << e->message << std::endl;
            return false;
        }

        return true;
}


int main (int argc, char *argv[]) {
    guint watcher_id;
    GMainLoop *loop;
    LMSProvider lms;
    BrowserProvider browser;
    PlayerProvider player;

    CommonAPI::Runtime::LoadState loadState;
    auto runtime = CommonAPI::Runtime::load(loadState);
    if (loadState != CommonAPI::Runtime::LoadState::SUCCESS) {
        std::cerr << "Error: Unable to load runtime!\n";
        return -1;
    }

    lms.connect([&](MmError *e) {
        registerService<IndexerStubImpl, LMSProvider> (runtime,
                                          &lms,
                                          "local:org.genivi.MediaManager.Indexer:"
                                          "org.genivi.MediaManager.Indexer",
                                          "Indexer",
                                          e);
    });

    browser.connect([&](MmError *e) {
        registerService<BrowserStubImpl, BrowserProvider> (runtime,
                                          &browser,
                                          "local:org.genivi.MediaManager.Browser:"
                                          "org.genivi.MediaManager.Browser",
                                          "Browser",
                                          e);
    });

    player.connect([&](MmError *e) {
        registerService<PlayerStubImpl, PlayerProvider> (runtime,
                                          &player,
                                          "local:org.genivi.MediaManager.Player:"
                                          "org.genivi.MediaManager.Player",
                                          "Player",
                                          e);
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}
