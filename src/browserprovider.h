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

/** \brief Provides the browsing functionality
 *
 *  This class provides all browsing functionality. Typically this means this
 *  class maintains connections to some backing service (for instance Rygel via
 *  dLeyna, as in the Media Manager PoC), and will call the appropriate
 *  functions in the backing service when functionality is called in this
 *  class.
 */
class BrowserProvider : public ServiceProvider {

public:
    BrowserProvider() : ServiceProvider("com.intel.dleyna-server") {}
    ~BrowserProvider() {}

    /**
     * Retrieve a list of all available Media Manager backends. The items in
     * the returned list should be treated as tokens, and should not be parsed,
     * since their structure may change.
     *
     * @param managers Vector of strings to populate
     * @param e Any potential error, unchanged if no error is set
     */
    void discoverMediaManagers (std::vector<std::string> &managers,
                                MmError **e);

    /** @description List all containers below the given path.
     *  @param path The path to search for containers. The format of the path
     *          depends on the underlying browsing backend
     *  @param offset: The offset to use for indexing the results list
     *  @param count: The number of results to return, starting from offset
     *  @param filter: Array of keys to include in resulting JSON elements
     *  @param containers A JSON list of all containers with the given path as parent
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void listContainers(std::string path,
                        uint64_t offset,
                        uint64_t count,
                        std::vector<std::string> filter,
                        std::string& containers,
                        MmError **e);

    /** @description Extends ListContainers with sorting capabilities
     *  @param sortKey Key to sort JSON result list on
     *  @see listContainers
     */
    void listContainersEx(std::string path,
                          uint64_t offset,
                          uint64_t count,
                          std::vector<std::string> filter,
                          std::string sortKey,
                          std::string& containers,
                          MmError **e);

    /** @description List all children in the given container
     *  @param path The path of the container to search for children.
     *          The format of the path depends on the underlying browsing
     *          backend
     *  @param offset The offset to use for indexing the results list
     *  @param count The number of results to return, starting from offset
     *  @param filter Array of keys to include in resulting JSON elements
     *  @param containers A JSON list of all children with the given container as parent
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void listChildren(std::string path,
                      uint64_t offset,
                      uint64_t count,
                      std::vector<std::string> filter,
                      std::string& children,
                      MmError **e);

    /** @description Extends ListChildren with sorting capabilities
     *  @param sortKey Key to sort JSON result list on
     *  @see listChildren
     */
    void listChildrenEx(std::string path,
                        uint64_t offset,
                        uint64_t count,
                        std::vector<std::string> filter,
                        std::string sortKey,
                        std::string& containers,
                        MmError **e);

    /** @description List all items in the given container
     *  @param path The path of the container to search for items.
     *          The format of the path depends on the underlying browsing
     *          backend
     *  @param offset The offset to use for indexing the results list
     *  @param count The number of results to return, starting from offset
     *  @param filter Array of keys to include in resulting JSON elements
     *  @param children A JSON list of all items with the given container as parent
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void listItems(std::string path,
                   uint64_t offset,
                   uint64_t count,
                   std::vector<std::string> filter,
                   std::string& items,
                   MmError **e);

    /** @description Extends ListItems with sorting capabilities
     *  @param sortKey Key to sort JSON result list on
     *  @see listChildren
     */
    void listItemsEx(std::string path,
                     uint64_t offset,
                     uint64_t count,
                     std::vector<std::string> filter,
                     std::string sortKey,
                     std::string& items,
                     MmError **e);

    /** @description Place a reference in a container, pointing to an object
     *  @param path Path to place the newly created container in
     *  @param objectPath Path to object to create reference to
     *  @param result Path identifier for the container containing the reference
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void createReference(std::string path,
                         std::string reference,
                         std::string& result,
                         MmError **e);

    /** @description Create a new empty container with path used as root
     *  @param path Path to place the newly created container in
     *  @param displayName Display name for the new container
     *  @param childTypes Array of allowed types on children in new container
     *  @param result Path identifier for the newly created container
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void createContainer(std::string path,
                         std::string displayname,
                         std::vector<std::string> childTypes,
                         std::string& result,
                         MmError **e);

    /** @description: Search for children in the given container
     *  @param path The path of the container to search for children.
     *          The format of the path depends on the underlying browsing
     *          backend
     *  @param query Search query to match children against. The format of the
     *          query depends on the underlying browsing backend
     *  @param offset: The offset to use for indexing the results list
     *  @param count: The number of results to return, starting from offset
     *  @param filter: Array of keys to include in resulting JSON elements
     *  @param objects A JSON list of all children matching the search criteria with
     *             the given container as parent
     *  @param e NO_CONNECTION if no connection can be established to underlying
     *            browsing engine
     *            BAD_PATH if path parameter is invalid
     */
    void searchObjects(std::string path,
                       std::string query,
                       uint64_t offset,
                       uint64_t count,
                       std::vector<std::string> filter,
                       std::string& objects,
                       MmError **e);

    /** @description Extends SearchObjects with sorting capabilities
     *  @param sortKey Key to sort JSON result list on
     *  @see searchObjects
     */
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
