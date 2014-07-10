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

#ifndef PLAYERPROVIDER_H
#define PLAYERPROVIDER_H

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

#include <gio/gio.h>
#include <jansson.h>

#include "common.h"
#include "serviceprovider.h"
#include "dleyna-renderer-generated.h"

class PlayerProvider : public ServiceProvider {

public:
    PlayerProvider() : ServiceProvider("com.intel.dleyna-renderer") {}
    ~PlayerProvider() {}
    void openURI(std::string uri,
                 MmError **e);

private:
bool connectMediaPlayer (const std::string path,
                         dleynaRendererMediaPlayer2Player **mc,
                         MmError **e);

};

#endif /* PLAYERPROVIDER_H */
