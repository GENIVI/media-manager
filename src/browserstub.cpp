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

#include "browserstub.h"
#include "common.h"

namespace MM = org::genivi::MediaManager;

BrowserStubImpl::BrowserStubImpl (BrowserProvider *browser) {
    m_browser = browser;
}

void BrowserStubImpl::discoverMediaManagers(std::vector<std::string> &idents,
                                            MM::Browser::BrowserError& e) {
    idents = discoverDLNABackends("servers", NULL);
}

void BrowserStubImpl::listContainers(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            std::string& containers,
                            MM::Browser::BrowserError& e) {

    m_browser->listContainers(path, offset, count, filter, containers, NULL);
}

void BrowserStubImpl::listItems(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            std::string& items,
                            MM::Browser::BrowserError& e) {

    m_browser->listItems(path, offset, count, filter, items, NULL);
}

void BrowserStubImpl::createReference(std::string path,
                         std::string reference,
                         std::string& result,
                         MM::Browser::BrowserError& e) {

    m_browser->createReference(path, reference, result, NULL);
}

void BrowserStubImpl::createContainer(std::string path,
                     std::string displayname,
                     std::vector<std::string> childTypes,
                     std::string& result,
                     MM::Browser::BrowserError& e) {
    m_browser->createContainer (path, displayname, childTypes, result, NULL);
}
