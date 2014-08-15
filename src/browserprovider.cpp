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

bool BrowserProvider::pathIsMediaManager(std::string path, MmError **e) {
    GError *error = NULL;
    GDBusProxy *proxy = NULL;
    GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                            NULL,
                                            &error);

    if (error) {
        if (e)
            (*e)->message = "Unable to get connection to bus";
        return false;
    }

    proxy = g_dbus_proxy_new_sync (conn,
                                   G_DBUS_PROXY_FLAGS_NONE,
                                   NULL,
                                   "com.intel.dleyna-server",
                                   path.c_str(),
                                   "org.freedesktop.DBus.Properties",
                                   NULL,
                                   &error);

    if (error) {
        if (e)
            (*e)->message = "Unable to create proxy for " + path;
        return false;
    }

    GVariant *ret = g_dbus_proxy_call_sync (proxy,
                                            "Get",
                                            g_variant_new("(ss)",
                                                          "org.gnome.UPnP.MediaObject2",
                                                          "DisplayName"),
                                            G_DBUS_CALL_FLAGS_NONE,
                                            -1,
                                            NULL,
                                            &error);

    if (error) {
        if (e)
            (*e)->message = "Unable to get property for " + path;
        return false;
    }

    GVariant *inner = g_variant_get_child_value (ret, 0);
    GVariant *inner2 = g_variant_get_variant (inner);
    const char *displayName = g_variant_get_string(inner2, 0);

    return g_strcmp0(displayName, MEDIA_MANAGER_IDENTIFIER) == 0;
}

void BrowserProvider::discoverMediaManagers(std::vector<std::string> &managers, MmError **e)
{
    GError *error = NULL;
    GDBusProxy *proxy = NULL;
    GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                            NULL,
                                            &error);

    if (error) {
        if (e)
            (*e)->message = "Unable to get connection to bus";
        return;
    }

    proxy = g_dbus_proxy_new_sync (conn,
                                   G_DBUS_PROXY_FLAGS_NONE,
                                   NULL,
                                   "com.intel.dleyna-server",
                                   "/com/intel/dLeynaServer",
                                   "com.intel.dLeynaServer.Manager",
                                   NULL,
                                   &error);

    if (error) {
        if (e)
            (*e)->message = "Unable to create proxy for com.intel.dLeynaServer.Manager";
        return;
    }

    GVariant *ret = g_dbus_proxy_call_sync (proxy,
                                            "GetServers",
                                            NULL,
                                            G_DBUS_CALL_FLAGS_NONE,
                                            -1,
                                            NULL,
                                            &error);

    ret = g_variant_get_child_value(ret, 0);

    gsize numObjs = 0;
    const char **objs = g_variant_get_objv (ret, &numObjs);

    for (gsize i = 0; i < numObjs; i++) {
        if (pathIsMediaManager(objs[i], e)) {
            managers.push_back (objs[i]);
        }
    }

    if (error) {
        if (e)
            (*e)->message = "Unable to get servers from dLeyna";
    }

}

void BrowserProvider::listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MmError **e)
{
    GError                *error  = NULL;
    GVariant              *out    = NULL;
    json_t                *object = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    gchar **filterStrv = stdStrvToStrv(filter);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;

    dleyna_server_media_container2_call_list_containers_sync (mc, offset, count,
                                                       filterStrv, &out, NULL,
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

void BrowserProvider::listItems(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& items,
                                MmError **e)
{
    GError                *error  = NULL;
    GVariant              *out    = NULL;
    json_t                *object = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    gchar **filterStrv = stdStrvToStrv(filter);

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;

    dleyna_server_media_container2_call_list_items_sync (mc, offset, count,
                                                  filterStrv, &out, NULL,
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

void BrowserProvider::createReference(std::string path,
                                      std::string reference,
                                      std::string& result,
                                      MmError **e)
{
    GError                      *error  = NULL;
    gchar                       *out    = NULL;
    dleynaServerMediaContainer2 *mc     = NULL;

    if (!BrowserProvider::connectMediaContainer(path, &mc, e))
        return;
    dleyna_server_media_container2_call_create_reference_sync (mc, reference.c_str(),
                                                              &out, NULL,
                                                              &error);

    if (error) {
        std::cout << "Error in listItems D-Bus call: " << error->message << std::endl;
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
        std::cout << "Error in listItems D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    result = out;
}

bool BrowserProvider::connectMediaContainer (const std::string path,
                                             dleynaServerMediaContainer2 **mc,
                                             MmError **e) {
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
