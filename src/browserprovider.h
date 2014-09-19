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

#ifndef BROWSERPROVIDER_H
#define BROWSERPROVIDER_H

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

#include <gio/gio.h>
#include <jansson.h>

#include "common.h"
#include "serviceprovider.h"
#include "dleyna-generated.h"


class BrowserProvider : public ServiceProvider {

public:
    BrowserProvider() : ServiceProvider("com.intel.dleyna-server") {}
    ~BrowserProvider() {}

    void discoverMediaManagers (std::vector<std::string> &managers,
                                MmError **e);

    void listContainers(std::string path,
                        uint64_t offset,
                        uint64_t count,
                        std::vector<std::string> filter,
                        std::string& containers,
                        MmError **e);

    void listContainersEx(std::string path,
                          uint64_t offset,
                          uint64_t count,
                          std::vector<std::string> filter,
                          std::string sortKey,
                          std::string& containers,
                          MmError **e);

    void listChildren(std::string path,
                      uint64_t offset,
                      uint64_t count,
                      std::vector<std::string> filter,
                      std::string& containers,
                      MmError **e);

    void listChildrenEx(std::string path,
                        uint64_t offset,
                        uint64_t count,
                        std::vector<std::string> filter,
                        std::string sortKey,
                        std::string& containers,
                        MmError **e);

    void listItems(std::string path,
                   uint64_t offset,
                   uint64_t count,
                   std::vector<std::string> filter,
                   std::string& items,
                   MmError **e);

    void listItemsEx(std::string path,
                     uint64_t offset,
                     uint64_t count,
                     std::vector<std::string> filter,
                     std::string sortKey,
                     std::string& items,
                     MmError **e);

    void createReference(std::string path,
                         std::string reference,
                         std::string& result,
                         MmError **e);

    void createContainer(std::string path,
                         std::string displayname,
                         std::vector<std::string> childTypes,
                         std::string& result,
                         MmError **e);

    void searchObjects(std::string path,
                       std::string query,
                       uint64_t offset,
                       uint64_t count,
                       std::vector<std::string> filter,
                       std::string& objects,
                       MmError **e);

    void searchObjectsEx(std::string path,
                         std::string query,
                         uint64_t offset,
                         uint64_t count,
                         std::vector<std::string> filter,
                         std::string sortKey,
                         std::string& objects,
                         MmError **e);

private:
    /**
     * Convert a DLNA dictionary, typically retrieved from dLeyna, to a JSON
     * dict.
     *
     * @param element DLNA dictionary
     *
     * @returns JSON object representing dict
     */
    bool connectMediaContainer (const std::string path,
                                dleynaServerMediaContainer2 **mc,
                                MmError **e);

    bool pathIsMediaManager(std::string, MmError **e);

    void listContainersGeneral (std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                std::string sortKey,
                                MmError **e);

    void listChildrenGeneral (std::string path,
                              uint64_t offset,
                              uint64_t count,
                              std::vector<std::string> filter,
                              std::string& containers,
                              std::string sortKey,
                              MmError **e);

    void searchObjectsGeneral(std::string path,
                              std::string query,
                              uint64_t offset,
                              uint64_t count,
                              std::vector<std::string> filter,
                              std::string sortKey,
                              std::string& objects,
                              MmError **e);

    void listItemsGeneral(std::string path,
                          uint64_t offset,
                          uint64_t count,
                          std::vector<std::string> filter,
                          std::string sortKey,
                          std::string& items,
                          MmError **e);
};

#endif /* BROWSERPROVIDER_H */
