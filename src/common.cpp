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

#include "common.h"
#include "unistd.h"
#include <string>
#include <algorithm>

#define MEDIA_MANAGER_IDENTIFIER "GENIVI MediaManager"


void Common::resultMapListToCAPIResultMapList(ResultMapList* from,
                                      MM::MediaTypes::ResultMapList& to,
                                      std::vector<std::string> filter) {

    for (size_t i = 0; i < json_array_size (from); i++) {
        json_t* currentObject = json_array_get(from, i);
        void* iter = json_object_iter (currentObject);
        MM::MediaTypes::ResultMap bm;
        while (iter) {
            const char* key = json_object_iter_key (iter);
            if (std::find( filter.begin(), filter.end(), key) != filter.end()) {
                json_t* value = json_object_iter_value (iter);
                std::pair<std::string, MM::MediaTypes::ResultUnion> elem;
                elem.first = key;

                if (json_is_string (value))
                    elem.second = MM::MediaTypes::ResultUnion (std::string(json_string_value(value)));
                else if (json_is_integer (value))
                    elem.second = MM::MediaTypes::ResultUnion ((int64_t)json_integer_value(value));
                else if (json_is_number (value))
                    elem.second = MM::MediaTypes::ResultUnion (json_number_value(value));
                else if (json_is_true (value))
                    elem.second = MM::MediaTypes::ResultUnion (true);
                else if (json_is_false (value))
                    elem.second = MM::MediaTypes::ResultUnion (false);
                else if (json_is_array (value)) {
                    std::vector<std::string> capiArr;
                    json_t *arr = value;
                    for (size_t i = 0; i < json_array_size (arr); i++) {
                        json_t* arrval = json_array_get (arr, i);
                        if (!arrval) {
                            std::cout << "Invalid array!" << std::endl;
                            break;
                        }
                        if (json_is_string (arrval)) {
                            capiArr.push_back (std::string(json_string_value(arrval)));
                        } else {
                            std::cout << "Only string arrays supported" << std::endl;
                        }
                    }
                    elem.second = capiArr;
                }
                else {
                    std::cout << "Unhandled key: " << key << std::endl;
                }
                bm.insert(elem);
            }
            iter = json_object_iter_next(currentObject, iter);
        }
        to.push_back(bm);
    }
}


/* Find "URLs" in Resources object. If there is one beginning with
 * "file://", pick that one, else pick any other. Put the selected URL in
 * the URI field */
void Common::postProcessJSONObjectResources (json_t* unprocessed) {
    std::vector<std::string> URLs;
    if (!json_is_object (unprocessed)) {
        std::cout << "Can't post-process. Not an object" << std::endl;
        return;
    }
    void* iter = json_object_iter (unprocessed);
    while (iter) {
        const char *key = json_object_iter_key (iter);
        json_t* value = json_object_iter_value (iter);

        if (g_strcmp0 (key, "Resources") == 0) {
            json_t* resourceArr = value;
            if (!json_is_array (resourceArr)) {
                std::cout << "Resources is not an array!" << std::endl;
                goto end_loop;
            }

            for (size_t i = 0; i < json_array_size (resourceArr); i++) {
                json_t* element = json_array_get (resourceArr, i);

                json_t* uri = json_object_get (element, "URL");
                if (uri) {
                    if (json_is_string (uri)) {
                        URLs.push_back (json_string_value (uri));
                    }
                }
            }
        }
        end_loop:
        iter = json_object_iter_next (unprocessed, iter);
    }

    for (size_t i = 0; i < URLs.size(); i++) {
        json_object_set_new (unprocessed, "URI", json_string(URLs.at(i).c_str()));
        if (URLs.at(i).find("file://") != std::string::npos)
            break;
    }
}

/** Change the JSON obtained from DLNA to match the JSON expected by the Media
 * Manager*/
void Common::postProcessJSON (json_t* unprocessed) {

    if (json_is_array(unprocessed)) {
        for (size_t i = 0; i < json_array_size(unprocessed); i++) {
            json_t* element = json_array_get (unprocessed, i);
            postProcessJSONObjectResources (element);
        }
    }
}

json_t * Common::DLNADictToJSON (GVariant *element) {
    if (!element) {
        std::cout << "Element is null" << std::endl;
        return NULL;
    }
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
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_INT32)) {
        return json_integer(g_variant_get_int32 (element));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_UINT64)) {
        return json_integer(g_variant_get_uint64 (element));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_INT64)) {
        return json_integer(g_variant_get_int64 (element));
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_VARIANT)) {
        GVariant *key   = NULL;
        key = g_variant_get_child_value (element, 0);
        return DLNADictToJSON(key);
    } else {
        return json_string((std::string("UNKNOWN: ") + g_variant_get_type_string (element)).c_str());
    }

    return NULL;
}

void Common::DLNAStringify(const json_t *object,
                   std::string &items,
                   MmError **e) {
    char *json = json_dumps(object, 0);
    if (!json) {
        std::string error = "JSON builder returned null!";
        std::cout << error << std::endl;
        if (e && *e)
            (*e)->message = error;
        return;
    }

    items = json;

    free(json);
}

gchar **Common::stdStrvToStrv(const std::vector<std::string> filter) {
    gchar **filterStrv = new gchar*[filter.size()+1];
    for (uint i = 0; i < filter.size(); i++) {
        filterStrv[i] = g_strdup(filter[i].c_str());
    }
    filterStrv[filter.size()] = NULL;

    return filterStrv;
}

bool Common::pathIsMediaManager(std::string type, std::string path, MmError **e) {
    GError *error = NULL;
    GDBusProxy *proxy = NULL;
    GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                            NULL,
                                            &error);

    std::string interface;
    std::string property;
    std::string name;

    if (type == "servers") {
        interface = "org.gnome.UPnP.MediaObject2";
        property = "DisplayName";
        name = "com.intel.dleyna-server";
    } else if (type == "renderers") {
        interface = "org.mpris.MediaPlayer2";
        property = "Identity";
        name = "com.intel.dleyna-renderer";
    } else {
        return false;
    }

    proxy = g_dbus_proxy_new_sync (conn,
                                   G_DBUS_PROXY_FLAGS_NONE,
                                   NULL,
                                   name.c_str(),
                                   path.c_str(),
                                   "org.freedesktop.DBus.Properties",
                                   NULL,
                                   &error);

    if (error) {
        if (e && *e)
            (*e)->message = "Unable to create proxy";
        return false;
    }

    GVariant *ret = g_dbus_proxy_call_sync (proxy,
                                            "Get",
                                            g_variant_new("(ss)",
                                                          interface.c_str(),
                                                          property.c_str()),
                                            G_DBUS_CALL_FLAGS_NONE,
                                            -1,
                                            NULL,
                                            &error);

    if (error) {
        if (e && *e)
            (*e)->message = "Unable to get property for " + path;
        return false;
    }

    GVariant *inner = g_variant_get_child_value (ret, 0);
    GVariant *inner2 = g_variant_get_variant (inner);
    const char *identity = g_variant_get_string(inner2, 0);

    if (type == "servers")
        return g_strcmp0(identity, MEDIA_MANAGER_IDENTIFIER) == 0;
    else if (type == "renderers") {
        char hostname[256];

        gethostname (hostname, 256);
        return g_strrstr(identity, hostname) != NULL;
    } else {
        return false;
    }

}

std::vector<std::string> Common::discoverDLNABackends(std::string type,
                                              MmError **e)
{
    GError *error = NULL;
    GDBusProxy *proxy = NULL;
    GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                            NULL,
                                            &error);
    std::vector<std::string> result;

    std::string name;
    std::string object_path;
    std::string interface;
    std::string method;

    if (type == "servers") {
        name = "com.intel.dleyna-server";
        object_path = "/com/intel/dLeynaServer";
        interface = "com.intel.dLeynaServer.Manager";
        method = "GetServers";
    } else if (type == "renderers") {
        name = "com.intel.dleyna-renderer";
        object_path = "/com/intel/dLeynaRenderer";
        interface = "com.intel.dLeynaRenderer.Manager";
        method = "GetRenderers";
    } else {
        return result;
    }

    if (error) {
        if (e && *e)
            (*e)->message = "Unable to get connection to bus";
        return result;
    }

    proxy = g_dbus_proxy_new_sync (conn,
                                   G_DBUS_PROXY_FLAGS_NONE,
                                   NULL,
                                   name.c_str(),
                                   object_path.c_str(),
                                   interface.c_str(),
                                   NULL,
                                   &error);

    if (error) {
        if (e && *e)
            (*e)->message = "Unable to create proxy";
        return result;
    }

    GVariant *ret = g_dbus_proxy_call_sync (proxy,
                                            method.c_str(),
                                            NULL,
                                            G_DBUS_CALL_FLAGS_NONE,
                                            -1,
                                            NULL,
                                            &error);

    ret = g_variant_get_child_value(ret, 0);

    gsize numObjs = 0;
    const char **objs = g_variant_get_objv (ret, &numObjs);

    for (gsize i = 0; i < numObjs; i++) {
        if (pathIsMediaManager(type, objs[i], e)) {
            result.push_back (objs[i]);
        }
    }

    if (error) {
        if (e && *e)
            (*e)->message = "Unable to get servers from dLeyna";
    }

}
