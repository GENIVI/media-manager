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
#include "indexerstub.h"



int main (int argc, char *argv[]) {
    guint watcher_id;
    GMainLoop *loop;
    LMSProvider lms;
    //lms.connect();

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

    std::cout << "Service publisher loaded!\n";

    auto indexerStub = std::make_shared<IndexerStubImpl>(&lms);
    const std::string commonApiAddress = "local:org.genivi.MediaManager.Indexer:"
                                         "org.genivi.MediaManager.Indexer";
    const bool success = servicePublisher->registerService(indexerStub,
                                                           commonApiAddress,
                                                           factory);
    if (!success) {
        std::cerr << "Error: Unable to register service!\n";
        return -1;
    }

    std::cout << "Service registration successful!\n";



    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}
