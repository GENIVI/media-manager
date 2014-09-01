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

#ifndef BROWSERSTUB_H
#define BROWSERSTUB_H

#include <CommonAPI/CommonAPI.h>

#include "../src-gen/org/genivi/MediaManager/BrowserStubDefault.h"

#include "browserprovider.h"

namespace MM = org::genivi::MediaManager;

class BrowserStubImpl: public org::genivi::MediaManager::BrowserStubDefault {

public:
    BrowserStubImpl (BrowserProvider *browser);
    void discoverMediaManagers(std::vector<std::string> &idents,
                               MM::Browser::BrowserError& e);
    void listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MM::Browser::BrowserError& e);

    void listItems(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            std::string& items,
                            MM::Browser::BrowserError& e);

    void createReference(std::string path,
                             std::string reference,
                             std::string& result,
                             MM::Browser::BrowserError& e);

    void createContainer(std::string path,
                         std::string displayname,
                         std::vector<std::string> childTypes,
                         std::string& result,
                         MM::Browser::BrowserError& e);

    void searchObjects(std::string path,
                       std::string query,
                       uint64_t offset,
                       uint64_t count,
                       std::vector<std::string> filter,
                       std::string& objects,
                       MM::Browser::BrowserError& e);

private:
    BrowserProvider *m_browser;
};

#endif /* BROWSERSTUB_H */
