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

#ifndef BROWSERPROVIDER_H
#define BROWSERPROVIDER_H

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

#include <gio/gio.h>

#include "common.h"
#include "serviceprovider.h"

class BrowserProvider : public ServiceProvider {

public:
    BrowserProvider() : ServiceProvider("com.intel.dleyna-server") {}
    ~BrowserProvider() {}

    virtual void listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MmError **e);
};

#endif /* BROWSERPROVIDER_H */
