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

    m_generalFilter.push_back("Path");
    m_generalFilter.push_back("Parent");
    m_generalFilter.push_back("Type");
    m_generalFilter.push_back("TypeEx");
    m_generalFilter.push_back("DisplayName");
    m_generalFilter.push_back("ChildCount");
    m_generalFilter.push_back("Artists");
    m_generalFilter.push_back("Artist");
    m_generalFilter.push_back("Searchable");
    m_generalFilter.push_back("MIMEType");
    m_generalFilter.push_back("Album");
    m_generalFilter.push_back("URI");
    m_generalFilter.push_back("Genre");
    m_generalFilter.push_back("Size");
    m_generalFilter.push_back("TrackNumber");
    m_generalFilter.push_back("Bitrate");
    m_generalFilter.push_back("SampleRate");
    m_generalFilter.push_back("Duration");
    m_generalFilter.push_back("AlbumArtURL");
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
    idents = Common::discoverDLNABackends("servers", NULL);
}

void BrowserStubImpl::listContainers(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            MM::MediaTypes::ResultMapList& containers,
                            MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();
    m_browser->listContainers(path,
                              offset,
                              count,
                              filter,
                              &bml,
                              NULL);

    Common::resultMapListToCAPIResultMapList(bml, containers, m_generalFilter);
}

void BrowserStubImpl::listChildrenEx(std::string path,
                                       uint64_t offset,
                                       uint64_t count,
                                       std::vector<std::string> filter,
                                       MM::BrowserTypes::SortKey sortKey,
                                       MM::MediaTypes::ResultMapList& children,
                                       MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->listChildrenEx(path,
                              offset,
                              count,
                              filter,
                              sortKeyToString(sortKey),
                              &bml,
                              NULL);

    Common::resultMapListToCAPIResultMapList(bml, children, m_generalFilter);
}

void BrowserStubImpl::listChildren(std::string path,
                                   uint64_t offset,
                                   uint64_t count,
                                   std::vector<std::string> filter,
                                   MM::MediaTypes::ResultMapList& children,
                                   MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->listChildren(path,
                            offset,
                            count,
                            filter,
                            &bml,
                            NULL);

    Common::resultMapListToCAPIResultMapList(bml, children, m_generalFilter);
}

void BrowserStubImpl::listContainersEx(std::string path,
                                       uint64_t offset,
                                       uint64_t count,
                                       std::vector<std::string> filter,
                                       MM::BrowserTypes::SortKey sortKey,
                                       MM::MediaTypes::ResultMapList& containers,
                                       MM::BrowserTypes::BrowserError& e) {

    Common::ResultMapList* bml = json_array();

    m_browser->listContainersEx(path,
                                offset,
                                count,
                                filter,
                                sortKeyToString(sortKey),
                                &bml,
                                NULL);

    Common::resultMapListToCAPIResultMapList(bml, containers, m_generalFilter);
}

void BrowserStubImpl::listItems(std::string path,
                            uint64_t offset,
                            uint64_t count,
                            std::vector<std::string> filter,
                            MM::MediaTypes::ResultMapList& items,
                            MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->listItems(path,
                         offset,
                         count,
                         filter,
                         &bml,
                         NULL);

    Common::resultMapListToCAPIResultMapList(bml, items, m_generalFilter);
}

void BrowserStubImpl::listItemsEx(std::string path,
                                  uint64_t offset,
                                  uint64_t count,
                                  std::vector<std::string> filter,
                                  MM::BrowserTypes::SortKey sortKey,
                                  MM::MediaTypes::ResultMapList& items,
                                  MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->listItemsEx(path,
                           offset,
                           count,
                           filter,
                           sortKeyToString(sortKey),
                           &bml,
                           NULL);

    Common::resultMapListToCAPIResultMapList(bml, items, m_generalFilter);
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
                                    MM::MediaTypes::ResultMapList& items,
                                    MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->searchObjects (path,
                              query,
                              offset,
                              count,
                              filter,
                              &bml,
                              NULL);

    Common::resultMapListToCAPIResultMapList(bml, items, m_generalFilter);
}

void BrowserStubImpl::searchObjectsEx(std::string path,
                                      std::string query,
                                      uint64_t offset,
                                      uint64_t count,
                                      std::vector<std::string> filter,
                                      MM::BrowserTypes::SortKey sortKey,
                                      MM::MediaTypes::ResultMapList& items,
                                      MM::BrowserTypes::BrowserError& e) {
    Common::ResultMapList* bml = json_array();

    m_browser->searchObjectsEx (path,
                                query,
                                offset,
                                count,
                                filter,
                                sortKeyToString(sortKey),
                                &bml,
                                NULL);

    Common::resultMapListToCAPIResultMapList(bml, items, m_generalFilter);
}
