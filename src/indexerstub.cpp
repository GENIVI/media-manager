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

#include "indexerstub.h"
#include <iostream>

IndexerStubImpl::IndexerStubImpl (LMSProvider *lms) {
    m_lms = lms;
}

void IndexerStubImpl::getDatabasePath (std::string &output) {
    std::cout << "Call to " << __FUNCTION__ << std::endl;
    output = m_lms->getDatabasePath();
}

void IndexerStubImpl::startIndexing() {
    std::cout << "Call to " << __FUNCTION__ << std::endl;
    m_lms->startIndexing();
}

void IndexerStubImpl::stopIndexing() {
    std::cout << "Call to " << __FUNCTION__ << std::endl;
    m_lms->stopIndexing();
}

const org::genivi::MediaManager::Indexer::IndexerStatus& IndexerStubImpl::getIndexerStatusAttribute () {
    return org::genivi::MediaManager::Indexer::IndexerStatus::RUNNING;
}
