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

gchar **BrowserProvider::stdStrvToStrv(const std::vector<std::string> filter) {
    gchar **filterStrv = new gchar*[filter.size()+1];
    for (uint i = 0; i < filter.size(); i++) {
        filterStrv[i] = g_strdup(filter[i].c_str());
    }
    filterStrv[filter.size()] = NULL;

    return filterStrv;
}
void BrowserProvider::DLNAStringify(const json_t *object,
                                    std::string &items,
                                    MmError **e) {
    char *json = json_dumps(object, 0);
    if (!json) {
        std::string error = "JSON builder returned null!";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return;
    }

    items = json;

    free(json);
}

json_t * BrowserProvider::DLNADictToJSON (GVariant *element) {
    const GVariantType *elementType = g_variant_get_type (element);
     if (g_variant_type_is_subtype_of (elementType, G_VARIANT_TYPE_DICTIONARY)) {
        GVariant     *key   = NULL, *value = NULL, *child = NULL;
        json_t       *obj   = json_object();
        GVariantIter  iter;

        g_variant_iter_init (&iter, element);
        while ((child = g_variant_iter_next_value (&iter))) {
            key = g_variant_get_child_value (child, 0);
            value = g_variant_get_child_value (child, 1);

            json_object_set_new (obj,
                                 g_variant_get_string(key, NULL),
                                 DLNADictToJSON (value));
        }

        return obj;
    } else if (g_variant_type_is_array (elementType)) {
        GVariantIter iter;
        GVariant *child = NULL;
        json_t *arr = json_array();

        g_variant_iter_init (&iter, element);
        while ((child = g_variant_iter_next_value (&iter))) {
            json_array_append_new(arr, DLNADictToJSON (child));
        }

        return arr;
    }else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_STRING)) {
        return json_string(g_variant_get_string (element, NULL));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_OBJECT_PATH)) {
        return json_string(g_variant_get_string (element, NULL));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_BOOLEAN)) {
        return json_boolean(g_variant_get_boolean (element));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_UINT32)) {
        return json_integer(g_variant_get_uint32 (element));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_VARIANT)) {
        GVariant *key   = NULL;
        key = g_variant_get_child_value (element, 0);
        return DLNADictToJSON(key);
    } else {
        return json_string((std::string("UNKNOWN: ") + g_variant_get_type_string (element)).c_str());
    }

    return NULL;
}
