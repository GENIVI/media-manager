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
void BrowserProvider::listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MmError **e)
{
    GError   *error  = NULL;
    GVariant *out    = NULL;
    char     *json   = NULL;
    json_t   *object = NULL;

    if (!g_variant_is_object_path (path.c_str())) {
        std::string error = "listContainers: Path is invalid";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return;
    }

    gchar **filterStrv = new gchar*[filter.size()+1];
    for (uint i = 0; i < filter.size(); i++) {
        filterStrv[i] = g_strdup(filter[i].c_str());
    }
    filterStrv[filter.size()] = NULL;

    dleynaMediaContainer2 *mc = dleyna_media_container2_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-server",
                                    path.c_str(),
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating proxy in listContainers: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }

    error = NULL;
    dleyna_media_container2_call_list_containers_sync (
        mc,
        offset,
        count,
        filterStrv,
        &out,
        NULL,
        &error);

    if (error) {
        std::cout << "Error in listContainers D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }


    object = DLNADictToJSON (out);
    json = json_dumps(object, 0);
    json_decref (object);
    if (!json) {
        std::string error = "JSON builder returned null!";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return;
    }

    containers = json;

    free(json);

    for (uint i = 0; i < filter.size(); i++) {
        free (filterStrv[i]);
    }
}
