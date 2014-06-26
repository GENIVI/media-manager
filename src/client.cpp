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

#include <iostream>
#include <future>
#include <CommonAPI/CommonAPI.h>

#include "../gen-src/org/genivi/mediamanager/IndexerProxy.h"


int main(int argc, char** argv) {
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


    const std::string commonApiAddress = "local:org.genivi.MediaManager.Indexer:org.genivi.MediaManager.Indexer";
    auto indexerProxy = factory->buildProxy<org::genivi::mediamanager::IndexerProxy>(commonApiAddress);
    if (!indexerProxy) {
        std::cerr << "Error: Unable to build proxy!\n";
        return -1;
    }

    std::cout << "Proxy created!\n";

    std::promise<CommonAPI::AvailabilityStatus> availabilityStatusPromise;
    indexerProxy->getProxyStatusEvent().subscribe([&](const CommonAPI::AvailabilityStatus& availabilityStatus) {
            availabilityStatusPromise.set_value(availabilityStatus);
            return CommonAPI::SubscriptionStatus::CANCEL;
            });

    auto availabilityStatusFuture = availabilityStatusPromise.get_future();
    std::cout << "Waiting for proxy availability...\n";
    availabilityStatusFuture.wait();

    if (availabilityStatusFuture.get() != CommonAPI::AvailabilityStatus::AVAILABLE) {
        std::cerr << "Proxy not available!\n";
        return -1;
    }

    std::cout << "Proxy available!\n";


    CommonAPI::CallStatus callStatus;
    std::string databasePath;

    indexerProxy->GetDatabasePath(callStatus, databasePath);
    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cerr << "Remote call failed!\n";
        return -1;
    }

    std::cout << "Got message: '" << databasePath << "'\n";

    return 0;
}
