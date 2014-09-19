#ifndef COMMON_H
#define COMMON_H

#include <gio/gio.h>
#include <jansson.h>
#include <string>
#include <iostream>
#include <vector>

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

json_t * DLNADictToJSON (GVariant *element);

void DLNAStringify(const json_t *object,
                   std::string &items,
                   MmError **e);

gchar **stdStrvToStrv(const std::vector<std::string> filter);

std::vector<std::string> discoverDLNABackends(std::string type,
                                              MmError **e);

#endif /* COMMON_H */
