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

#include "../src-gen/org/genivi/mediamanager/BrowserStubDefault.h"

#include "browserprovider.h"

namespace MM = org::genivi::mediamanager;

class BrowserStubImpl: public org::genivi::mediamanager::BrowserStubDefault {

public:
    BrowserStubImpl (BrowserProvider *browser);
    void discoverMediaManagers(std::vector<std::string> &idents,
                               MM::BrowserTypes::BrowserError& e);

    void listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MM::BrowserTypes::BrowserError& e);

    void listContainersEx(std::string path,
                          uint64_t offset,
                          uint64_t count,
                          std::vector<std::string> filter,
                          MM::BrowserTypes::SortKey sortKey,
                          std::string& children,
                          MM::BrowserTypes::BrowserError& e);

    void listChildren(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& children,
                                MM::BrowserTypes::BrowserError& e);

    void listChildrenEx(std::string path,
                          uint64_t offset,
                          uint64_t count,
                          std::vector<std::string> filter,
                          MM::BrowserTypes::SortKey sortKey,
                          std::string& containers,
                          MM::BrowserTypes::BrowserError& e);

    void listItems(std::string path,
                   uint64_t offset,
                   uint64_t count,
                   std::vector<std::string> filter,
                   std::string& items,
                   MM::BrowserTypes::BrowserError& e);

    void listItemsEx(std::string path,
                     uint64_t offset,
                     uint64_t count,
                     std::vector<std::string> filter,
                     MM::BrowserTypes::SortKey sortKey,
                     std::string& items,
                     MM::BrowserTypes::BrowserError& e);

    void createReference(std::string path,
                         std::string reference,
                         std::string& result,
                         MM::BrowserTypes::BrowserError& e);

    void createContainer(std::string path,
                         std::string displayname,
                         std::vector<std::string> childTypes,
                         std::string& result,
                         MM::BrowserTypes::BrowserError& e);

    void searchObjects(std::string path,
                       std::string query,
                       uint64_t offset,
                       uint64_t count,
                       std::vector<std::string> filter,
                       std::string& objects,
                       MM::BrowserTypes::BrowserError& e);

    void searchObjectsEx(std::string path,
                         std::string query,
                         uint64_t offset,
                         uint64_t count,
                         std::vector<std::string> filter,
                         MM::BrowserTypes::SortKey sortKey,
                         std::string& objects,
                         MM::BrowserTypes::BrowserError& e);

private:
    BrowserProvider *m_browser;
};

#endif /* BROWSERSTUB_H */
