 /*
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

#include "indexerstub.h"
#include "common.h"

namespace MM = org::genivi::mediamanager;

IndexerStubImpl::IndexerStubImpl (LMSProvider *lms) {
    m_lms = lms;
}

void IndexerStubImpl::getDatabasePath (std::string &output, MM::Indexer::IndexerError &e) {
    MmError *error = NULL;

    std::cout << "Call to " << __FUNCTION__ << std::endl;
    m_lms->getDatabasePath(output, &error);

    if (error) {
        std::cout << "Setting error to BACKEND_UNREACHABLE" << std::endl;
        e = MM::Indexer::IndexerError::BACKEND_UNREACHABLE;
    }
}

void IndexerStubImpl::startIndexing(MM::Indexer::IndexerError &e) {
    MmError *error = NULL;

    std::cout << "Call to " << __FUNCTION__ << std::endl;
    m_lms->startIndexing(&error);

    if (error) {
        std::cout << "Setting error to BACKEND_UNREACHABLE" << std::endl;
        e = MM::Indexer::IndexerError::BACKEND_UNREACHABLE;
    }
}

void IndexerStubImpl::stopIndexing(MM::Indexer::IndexerError &e) {
    MmError *error = NULL;

    std::cout << "Call to " << __FUNCTION__ << std::endl;
    m_lms->stopIndexing(&error);

    if (error) {
        std::cout << "Setting error to BACKEND_UNREACHABLE" << std::endl;
        e = MM::Indexer::IndexerError::BACKEND_UNREACHABLE;
    }
}

const MM::Indexer::IndexerStatus& IndexerStubImpl::getIndexerStatusAttribute () {
    std::cout << "Not implemented: Call to " << __FUNCTION__ << std::endl;
    return MM::Indexer::IndexerStatus::RUNNING;
}
