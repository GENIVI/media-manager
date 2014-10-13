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

#include "browserstub.h"
#include "common.h"

namespace MM = org::genivi::mediamanager;

BrowserStubImpl::BrowserStubImpl (BrowserProvider *browser) {
    m_browser = browser;
}

std::string sortKeyToString (MM::BrowserTypes::SortKey sk) {
    std::string keyStr;
    if (sk.order == MM::BrowserTypes::SortOrder::ASCENDING)
        keyStr += "+";
    else
        keyStr += "-";

    keyStr += sk.keyName;

    return keyStr;
}

void BrowserStubImpl::discoverMediaManagers(std::vector<std::string> &idents,
                                            MM::BrowserTypes::BrowserError& e) {
    idents = discoverDLNABackends("servers", NULL);
}

void BrowserStubImpl::listContainers(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            std::string& containers,
                            MM::BrowserTypes::BrowserError& e) {

    m_browser->listContainers(path,
                              offset,
                              count,
                              filter,
                              containers,
                              NULL);
}

void BrowserStubImpl::listChildrenEx(std::string path,
                                       uint64_t offset,
                                       uint64_t count,
                                       std::vector<std::string> filter,
                                       MM::BrowserTypes::SortKey sortKey,
                                       std::string& children,
                                       MM::BrowserTypes::BrowserError& e) {

    m_browser->listChildrenEx(path,
                              offset,
                              count,
                              filter,
                              sortKeyToString(sortKey),
                              children,
                              NULL);
}

void BrowserStubImpl::listChildren(std::string path,
                                   uint64_t offset,
                                   uint64_t count,
                                   std::vector<std::string> filter,
                                   std::string& children,
                                   MM::BrowserTypes::BrowserError& e) {

    m_browser->listChildren(path,
                            offset,
                            count,
                            filter,
                            children,
                            NULL);
}

void BrowserStubImpl::listContainersEx(std::string path,
                                       uint64_t offset,
                                       uint64_t count,
                                       std::vector<std::string> filter,
                                       MM::BrowserTypes::SortKey sortKey,
                                       std::string& containers,
                                       MM::BrowserTypes::BrowserError& e) {

    m_browser->listContainersEx(path,
                                offset,
                                count,
                                filter,
                                sortKeyToString(sortKey),
                                containers,
                                NULL);
}

void BrowserStubImpl::listItems(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            std::string& items,
                            MM::BrowserTypes::BrowserError& e) {

    m_browser->listItems(path,
                         offset,
                         count,
                         filter,
                         items,
                         NULL);
}

void BrowserStubImpl::listItemsEx(std::string path,
                                  uint64_t offset,
                                  uint64_t count,
                                  std::vector<std::string> filter,
                                  MM::BrowserTypes::SortKey sortKey,
                                  std::string& items,
                                  MM::BrowserTypes::BrowserError& e) {

    m_browser->listItemsEx(path,
                           offset,
                           count,
                           filter,
                           sortKeyToString(sortKey),
                           items,
                           NULL);
}

void BrowserStubImpl::createReference(std::string path,
                         std::string reference,
                         std::string& result,
                         MM::BrowserTypes::BrowserError& e) {

    m_browser->createReference(path,
                               reference,
                               result,
                               NULL);
}

void BrowserStubImpl::createContainer(std::string path,
                     std::string displayname,
                     std::vector<std::string> childTypes,
                     std::string& result,
                     MM::BrowserTypes::BrowserError& e) {
    m_browser->createContainer (path,
                                displayname,
                                childTypes,
                                result,
                                NULL);
}

void BrowserStubImpl::searchObjects(std::string path,
                                    std::string query,
                                    uint64_t offset,
                                    uint64_t count,
                                    std::vector<std::string> filter,
                                    std::string& objects,
                                    MM::BrowserTypes::BrowserError& e) {
    m_browser->searchObjects (path,
                              query,
                              offset,
                              count,
                              filter,
                              objects,
                              NULL);
}

void BrowserStubImpl::searchObjectsEx(std::string path,
                                      std::string query,
                                      uint64_t offset,
                                      uint64_t count,
                                      std::vector<std::string> filter,
                                      MM::BrowserTypes::SortKey sortKey,
                                      std::string& objects,
                                      MM::BrowserTypes::BrowserError& e) {
    m_browser->searchObjectsEx (path,
                                query,
                                offset,
                                count,
                                filter,
                                sortKeyToString(sortKey),
                                objects,
                                NULL);
}
