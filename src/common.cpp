#include "common.h"
#include "unistd.h"

#define MEDIA_MANAGER_IDENTIFIER "GENIVI MediaManager"

json_t * DLNADictToJSON (GVariant *element) {
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
    } else if (g_variant_type_equal (elementType, G_VARIANT_TYPE_VARIANT)) {
        GVariant *key   = NULL;
        key = g_variant_get_child_value (element, 0);
        return DLNADictToJSON(key);
    } else {
        return json_string((std::string("UNKNOWN: ") + g_variant_get_type_string (element)).c_str());
    }

    return NULL;
}

void DLNAStringify(const json_t *object,
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

gchar **stdStrvToStrv(const std::vector<std::string> filter) {
    gchar **filterStrv = new gchar*[filter.size()+1];
    for (uint i = 0; i < filter.size(); i++) {
        filterStrv[i] = g_strdup(filter[i].c_str());
    }
    filterStrv[filter.size()] = NULL;

    return filterStrv;
}

bool pathIsMediaManager(std::string type, std::string path, MmError **e) {
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
        if (e)
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
        if (e)
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

std::vector<std::string> discoverDLNABackends(std::string type,
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
        if (e)
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
        if (e)
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
        if (e)
            (*e)->message = "Unable to get servers from dLeyna";
    }

}
