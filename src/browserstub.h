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

#include <CommonAPI/CommonAPI.h>

#include "../src-gen/org/genivi/MediaManager/BrowserStubDefault.h"

#include "browserprovider.h"

namespace MM = org::genivi::MediaManager;

class BrowserStubImpl: public org::genivi::MediaManager::BrowserStubDefault {

public:
    BrowserStubImpl (BrowserProvider *browser);
    virtual void listContainers(std::string path,
                                uint64_t offset,
                                uint64_t count,
                                std::vector<std::string> filter,
                                std::string& containers,
                                MM::Browser::BrowserError& e);

private:
    BrowserProvider *m_browser;
};
