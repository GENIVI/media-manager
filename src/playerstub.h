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

#ifndef PLAYERSTUB_H
#define PLAYERSTUB_H

#include <CommonAPI/CommonAPI.h>

#include "../src-gen/org/genivi/MediaManager/PlayerStubDefault.h"

#include "playerprovider.h"

namespace MM = org::genivi::MediaManager;

class PlayerStubImpl: public org::genivi::MediaManager::PlayerStubDefault {

public:
    PlayerStubImpl (PlayerProvider *browser);

    virtual void openUri(std::string uri, MM::Player::PlayerError& e);

private:
    PlayerProvider *m_player;
};

#endif /* PLAYERSTUB_H */
