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
#include "indexerstub.h"
#include "browserstub.h"



int main (int argc, char *argv[]) {
    guint watcher_id;
    GMainLoop *loop;
    LMSProvider lms;
    BrowserProvider browser;

    CommonAPI::Runtime::LoadState loadState;
    auto runtime = CommonAPI::Runtime::load(loadState);
    if (loadState != CommonAPI::Runtime::LoadState::SUCCESS) {
        std::cerr << "Error: Unable to load runtime!\n";
        return -1;
    }

    std::cout << "Runtime loaded!\n";


    auto factory = runtime->createFactory();
    if (!factory) {
        std::cerr << "Error: Unable to create factory!\n";
        return -1;
    }

    std::cout << "Factory created!\n";


    auto servicePublisher = runtime->getServicePublisher();
    if (!servicePublisher) {
        std::cerr << "Error: Unable to load service publisher!\n";
        return -1;
    }

    lms.connect([&](MmError *e) {
        if (!e) {
            auto indexerStub = std::make_shared<IndexerStubImpl>(&lms);
            const std::string commonApiAddressIndexer = "local:org.genivi.MediaManager.Indexer:"
                                                        "org.genivi.MediaManager.Indexer";
            const bool successIndexer = servicePublisher->registerService(indexerStub,
                                                                   commonApiAddressIndexer,
                                                                   factory);
            if (!successIndexer) {
                std::cerr << "Error: Unable to register Indexer service!\n";
            }
        } else {
            std::cout << "Error connecting to LMS: " << e->message << std::endl;
        }
    });

    browser.connect([&](MmError *e) {
        if (!e) {
            auto browserStub = std::make_shared<BrowserStubImpl>(&browser);
            const std::string commonApiAddressBrowser = "local:org.genivi.MediaManager.Browser:"
                                                        "org.genivi.MediaManager.Browser";
            const bool successBrowser = servicePublisher->registerService(browserStub,
                                                                   commonApiAddressBrowser,
                                                                   factory);
            if (!successBrowser) {
                std::cerr << "Error: Unable to register Browser service!\n";
                return -1;
            }
        } else {
            std::cout << "Error connecting to Browser: " << e->message << std::endl;
        }
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}
