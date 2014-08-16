#include "common.h"

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
