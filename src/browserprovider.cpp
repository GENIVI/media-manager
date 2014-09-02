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
#include <sstream>

#include "browserprovider.h"

void BrowserProvider::listContainersGeneral (std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                std::string sortKey,
                                MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                *error  = NULL;
    GVariant              *out    = NULL;
    json_t                *object = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    gchar **filterStrv = stdStrvToStrv(filter);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;

    if (sortKey == "")
        dleyna_server_media_container2_call_list_containers_sync (mc, offset, count,
                                                           filterStrv, &out, NULL,
                                                           &error);
    else
        dleyna_server_media_container2_call_list_containers_ex_sync (mc, offset, count,
                                                           filterStrv, sortKey.c_str(), &out, NULL,
                                                           &error);

    if (error) {
        std::cout << "Error in listContainers D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    object = DLNADictToJSON (out);
    DLNAStringify(object, containers, e);

    for (uint i = 0; i < filter.size(); i++) {
        free (filterStrv[i]);
    }
}

void BrowserProvider::listContainers(std::string path,
                                     uint64_t offset,
                                     uint64_t count,
                                     std::vector<std::string> filter,
                                     std::string& containers,
                                     MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    listContainersGeneral(path, offset, count, filter, containers, "", e);
}

void BrowserProvider::listContainersEx(std::string path,
                                       uint64_t offset,
                                       uint64_t count,
                                       std::vector<std::string> filter,
                                       std::string sortKey,
                                       std::string& containers,
                                       MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    listContainersGeneral(path, offset, count, filter, containers, sortKey, e);
}

void BrowserProvider::listItemsGeneral (std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string sortKey,
                                std::string& items,
                                MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                *error  = NULL;
    GVariant              *out    = NULL;
    json_t                *object = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    gchar **filterStrv = stdStrvToStrv(filter);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;

    if (sortKey == "")
        dleyna_server_media_container2_call_list_items_sync (mc, offset, count,
                                                      filterStrv, &out, NULL,
                                                      &error);
    else
        dleyna_server_media_container2_call_list_items_ex_sync (mc, offset, count,
                                                      filterStrv, sortKey.c_str(), &out, NULL,
                                                      &error);

    if (error) {
        std::cout << "Error in listItems D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    object = DLNADictToJSON (out);
    DLNAStringify (object, items, e);
    json_decref (object);

    for (uint i = 0; i < filter.size(); i++) {
        free (filterStrv[i]);
    }
}
void BrowserProvider::listItems(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& items,
                                MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    listItemsGeneral (path, offset, count, filter, "", items, e);
}
void BrowserProvider::listItemsEx(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string sortKey,
                                std::string& items,
                                MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    listItemsGeneral (path, offset, count, filter, sortKey, items, e);
}

void BrowserProvider::createReference(std::string path,
                                      std::string reference,
                                      std::string& result,
                                      MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                      *error  = NULL;
    gchar                       *out    = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;
    dleyna_server_media_container2_call_create_reference_sync (mc, reference.c_str(),
                                                              &out, NULL,
                                                              &error);

    if (error) {
        std::cout << "Error in createReference D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }
}
void BrowserProvider::createContainer(std::string path,
                     std::string displayname,
                     std::vector<std::string> childTypes,
                     std::string& result,
                     MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                      *error  = NULL;
    gchar                       *out    = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    gchar **ct = stdStrvToStrv (childTypes);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;
    dleyna_server_media_container2_call_create_container_sync (mc,
                                                              displayname.c_str(),
                                                              "container",
                                                              ct,
                                                              &out, NULL,
                                                              &error);

    if (error) {
        std::cout << "Error in createContainer D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    result = out;
}

void BrowserProvider::searchObjectsGeneral(std::string path,
                                    std::string query,
                                    uint64_t offset,
                                    uint64_t count,
                                    std::vector<std::string> filter,
                                    std::string sortKey,
                                    std::string& objects,
                                    MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                      *error  = NULL;
    GVariant                    *out    = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;
    json_t                      *object = NULL;
    guint                        total_items;

    gchar **flt = stdStrvToStrv (filter);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;

    if (sortKey == "")
        dleyna_server_media_container2_call_search_objects_sync (mc,
                                                                 query.c_str(),
                                                                 offset,
                                                                 count,
                                                                 flt,
                                                                 &out, NULL,
                                                                 &error);
    else
        dleyna_server_media_container2_call_search_objects_ex_sync (mc,
                                                                 query.c_str(),
                                                                 offset,
                                                                 count,
                                                                 flt,
                                                                 sortKey.c_str(),
                                                                 &out,
                                                                 &total_items,
                                                                 NULL,
                                                                 &error);

    if (error) {
        std::cout << "Error in searchObjects D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    object = DLNADictToJSON (out);
    DLNAStringify (object, objects, e);
    json_decref (object);

    for (uint i = 0; i < filter.size(); i++) {
        free (flt[i]);
    }
}
void BrowserProvider::searchObjects(std::string path,
                                    std::string query,
                                    uint64_t offset,
                                    uint64_t count,
                                    std::vector<std::string> filter,
                                    std::string& objects,
                                    MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    searchObjectsGeneral (path, query, offset, count, filter, "", objects, e);
}
void BrowserProvider::searchObjectsEx(std::string path,
                                      std::string query,
                                      uint64_t offset,
                                      uint64_t count,
                                      std::vector<std::string> filter,
                                      std::string sortKey,
                                      std::string& objects,
                                      MmError **e)
{
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    searchObjectsGeneral (path, query, offset, count, filter, sortKey, objects, e);
}

bool BrowserProvider::connectMediaContainer (const std::string path,
                                             dleynaServerMediaContainer2 **mc,
                                             MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError *error = NULL;

    if (!g_variant_is_object_path (path.c_str())) {
        std::string error = "Path is invalid";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return false;
    }


    *mc = dleyna_server_media_container2_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-server",
                                    path.c_str(),
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating MediaContainer2 proxy: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return false;
    }

    return true;
}
