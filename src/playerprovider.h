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
#include "dleyna-generated.h"
#include "../src-gen/org/genivi/MediaManager/PlayerStubDefault.h"

class PlayerProvider : public ServiceProvider {
friend class PlayerStubImpl;

public:
    PlayerProvider() :
        ServiceProvider("com.intel.dleyna-renderer"),
        playQueuePosition(0),
        stub(0),
        mp(0),
        mc(0),
        playqueue(0),
        isPlaying(0),
        m_signalHandlerId(0),
        m_repeat(0),
        m_shuffle(0)
        {}
    ~PlayerProvider() {}
    void openURI(std::string uri, MmError **e);
    void pause(MmError **e);
    void play(MmError **e);
    void playPause(MmError **e);
    void openPlaylist (std::string, MmError **e);
    void next (MmError **e);
    void previous (MmError **e);
    void setRate (double rate, MmError **e);
    void setRepeat (bool);
    void setShuffle (bool);

    void handlePropertyChangedSignal (std::string, GVariant *);
    org::genivi::MediaManager::PlayerStubDefault *stub;
private:
dleynaRendererMediaPlayer2Player *mp;
dleynaServerMediaContainer2      *mc;
int playQueuePosition;
json_t *playqueue;
bool isPlaying;
guint m_signalHandlerId;
bool m_repeat;
bool m_shuffle;

bool registerSignalListener(std::string);

bool connectMediaPlayer (const std::string path,
                         dleynaRendererMediaPlayer2Player **mc,
                         MmError **e);

bool connectMediaContainer(const std::string path,
                           dleynaServerMediaContainer2 **mc,
                           MmError **e);

char *findFirstPlayer(MmError **e);

bool changePlayQueuePosition (int increment, MmError **e);

std::string getDisplayName (json_t *item, bool &ok);
std::string getLocalURL (json_t *item, bool &ok);
};

#endif /* PLAYERPROVIDER_H */
