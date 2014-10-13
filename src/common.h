#ifndef COMMON_H
#define COMMON_H

#include <gio/gio.h>
#include <jansson.h>
#include <string>
#include <iostream>
#include <vector>

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

/**
 * Convert DLNA dict (retrieved from dLeyna) to JSON
 *
 * @param element DLNA dict
 * @return JSON representation of input dict
 */
json_t * DLNADictToJSON (GVariant *element);

/**
 * Convert a JSON representation of a DLNA dict to a string
 * @param object The JSON object to convert
 * @param items the output string for the result
 * @param e Error object, set on failure, unchanged on success
 */
void DLNAStringify(const json_t *object,
                   std::string &items,
                   MmError **e);

/**
 * Convert a string vector to a null-terminated string array
 * @param filter String vector to convert
 * @return null terminated string array
 */
gchar **stdStrvToStrv(const std::vector<std::string> filter);


/**
 * Helper function for discovering DLNA backends
 *
 * @param type 'servers' or 'renderers'
 * @param e Set to an error on failure, unchanged on success
 * @return Vector of paths to renderers or servers, depending on input
 */
std::vector<std::string> discoverDLNABackends(std::string type,
                                              MmError **e);

#endif /* COMMON_H */
