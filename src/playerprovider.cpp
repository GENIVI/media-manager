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
#include "string.h"

#include "playerprovider.h"

static char* PLAYER_PATH = "/com/intel/dLeynaRenderer/server/0";

char *PlayerProvider::findFirstPlayer(MmError **e) {
    char *path = NULL;
    dleynaRendererOrgFreedesktopDBusPeer *peer = NULL;
    int slashPos = -1;
    for (int i = strlen(PLAYER_PATH); i > 0; i--) {
        if (PLAYER_PATH[i] == '/') {
            slashPos = i;
            break;
        }
    }

    path = g_strndup(PLAYER_PATH, slashPos);

    for (int i = 0; i < 50; i++) {
        char *newpath = g_strdup_printf("%s/%d", path, i);

        GError *error = NULL;
        GDBusProxy *proxy = NULL;
        GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                                NULL,
                                                &error);

        if (error) {
            if (e)
                (*e)->message = "Unable to get connection to bus";
            return NULL;
        }

        proxy = g_dbus_proxy_new_sync (conn,
                                       G_DBUS_PROXY_FLAGS_NONE,
                                       NULL,
                                       "com.intel.dleyna-renderer",
                                       newpath,
                                       "org.freedesktop.DBus.Properties",
                                       NULL,
                                       &error);

        if (error) {
            if (e)
                (*e)->message = std::string("Unable to create proxy for ") + newpath;
            return NULL;
        }

        GVariant *ret = g_dbus_proxy_call_sync (proxy,
                                                "Get",
                                                g_variant_new("(ss)",
                                                              "org.mpris.MediaPlayer2",
                                                              "Identity"),
                                                G_DBUS_CALL_FLAGS_NONE,
                                                -1,
                                                NULL,
                                                &error);
        if (!error) {
            g_printerr ("Found player on %s\n", newpath);
            return newpath;
            break;
        } else {
            g_printerr ("No player found on %s\n", newpath);
        }
    }

    g_free (path);
    return NULL;
}

void PlayerProvider::openURI(std::string uri,
                             MmError **e)
{
    GError                           *error  = NULL;
    dleynaRendererMediaPlayer2Player *mp     = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_open_uri_sync (mp,
                                                             uri.c_str(),
                                                             NULL,
                                                             &error);

    checkError (error, e);
}

void PlayerProvider::pause(MmError **e) {
    GError                           *error = NULL;
    dleynaRendererMediaPlayer2Player *mp    = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_pause_sync (mp,
                                                             NULL,
                                                             &error);

    checkError (error, e);
}

void PlayerProvider::play(MmError **e) {
    GError                           *error = NULL;
    dleynaRendererMediaPlayer2Player *mp    = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_play_sync (mp,
                                                         NULL,
                                                         &error);

    checkError (error, e);
}

void PlayerProvider::playPause(MmError **e) {
    GError                           *error = NULL;
    dleynaRendererMediaPlayer2Player *mp    = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_play_pause_sync (mp,
                                                               NULL,
                                                               &error);

    checkError (error, e);
}

bool PlayerProvider::connectMediaPlayer (const std::string path,
                                             dleynaRendererMediaPlayer2Player **mp,
                                             MmError **e) {
    GError *error = NULL;

    char *foundpath = findFirstPlayer(NULL);
    if (!foundpath) {
        g_printerr ("Unable to find any player");
        return false;
    }

    if (!g_variant_is_object_path (foundpath)) {
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
                                    foundpath,
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
