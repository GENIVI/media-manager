#ifndef COMMON_H
#define COMMON_H

#include <gio/gio.h>
#include <jansson.h>
#include <string>
#include <iostream>
#include <vector>

#include "../src-gen/org/genivi/mediamanager/MediaTypes.h"

namespace MM = org::genivi::mediamanager;

/**
 * Class for errors in the Media Manager
 */
class MmError {
public:
MmError () {
    this->message = "";
}

MmError (std::string msg) {
    this->message = msg;
}

std::string message;
};

class Common {
public:
typedef json_t ResultMapList;

static void resultMapListToCAPIResultMapList(ResultMapList* from,
                                      MM::MediaTypes::ResultMapList& to,
                                      std::vector<std::string> filter);

static void postProcessJSON (json_t* unprocessed);
/**
 * Convert DLNA dict (retrieved from dLeyna) to JSON
 *
 * @param element DLNA dict
 * @return JSON representation of input dict
 */
static json_t * DLNADictToJSON (GVariant *element);

/**
 * Convert a JSON representation of a DLNA dict to a string
 * @param object The JSON object to convert
 * @param items the output string for the result
 * @param e Error object, set on failure, unchanged on success
 */
static void DLNAStringify(const json_t *object,
                   std::string &items,
                   MmError **e);

/**
 * Convert a string vector to a null-terminated string array
 * @param filter String vector to convert
 * @return null terminated string array
 */
static gchar **stdStrvToStrv(const std::vector<std::string> filter);


/**
 * Helper function for discovering DLNA backends
 *
 * @param type 'servers' or 'renderers'
 * @param e Set to an error on failure, unchanged on success
 * @return Vector of paths to renderers or servers, depending on input
 */
static std::vector<std::string> discoverDLNABackends(std::string type,
                                              MmError **e);

private:
static void postProcessJSONObjectResources (json_t* unprocessed);
static bool pathIsMediaManager(std::string, std::string, MmError**);

};
#endif /* COMMON_H */
