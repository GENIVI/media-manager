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

#include <iostream>
#include <sstream>

#include "playerprovider.h"

#define PLAYER_PATH "/com/intel/dLeynaRenderer/server/1"

void PlayerProvider::openURI(std::string uri,
                             MmError **e)
{
    GError                           *error  = NULL;
    dleynaRendererMediaPlayer2Player *mp     = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_open_uri_sync (mp, uri.c_str(), NULL,
                                                  &error);

    if (error) {
        std::cout << "Error in listItems D-Bus call: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return;
    }
}

bool PlayerProvider::connectMediaPlayer (const std::string path,
                                             dleynaRendererMediaPlayer2Player **mp,
                                             MmError **e) {
    GError *error = NULL;

    if (!g_variant_is_object_path (path.c_str())) {
        std::string error = "Path is invalid";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return false;
    }


    *mp = dleyna_renderer_media_player2_player_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-renderer",
                                    path.c_str(),
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating MediaPlayer2 proxy: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return false;
    }

    return true;
}
