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

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_pause_sync (mp,
                                                             NULL,
                                                             &error);

    // dLeyna says we failed to pause, but PAUSED to the client anyway
    if (!checkError (error, e)) {
        this->stub->setPlaybackStatusAttribute(org::genivi::MediaManager::Player::PlaybackStatus::PAUSED);
    }
}

void PlayerProvider::play(MmError **e) {
    GError                           *error = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_play_sync (mp,
                                                         NULL,
                                                         &error);

    checkError (error, e);
}

void PlayerProvider::playPause(MmError **e) {
    GError                           *error = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_play_pause_sync (mp,
                                                               NULL,
                                                               &error);

    checkError (error, e);
}

void PlayerProvider::openPlaylist (std::string playlistPath, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                           *error = NULL;
    GVariant *out = NULL;

    std::vector<std::string> filter;
    filter.push_back("*");

    int offset = 0;
    int count = 100;
    gchar **filterStrv = stdStrvToStrv(filter);

    if (!PlayerProvider::connectMediaContainer (playlistPath, &mc, e))
        return;

    dleyna_server_media_container2_call_list_items_sync (mc, offset, count,
                                                  filterStrv, &out, NULL,
                                                  &error);

    if (error) {
        std::cout << "Unable to open playlist: " << error->message << std::endl;
        return;
    }

    std::string containers;
    std::cout << "Setting play queue to: " << playlistPath << std::endl;

    json_decref (playqueue);
    playqueue = DLNADictToJSON (out);

    playQueuePosition = 0;
    changePlayQueuePosition (0, e);

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

    if (!registerSignalListener(foundpath)) {
        std::cout << "Failed to set up signal listener";
        return false;
    }

    return true;
}

void playerprovider_dbus_properties_changed_cb (GDBusConnection *connection,
                                                const gchar *sender_name,
                                                const gchar *object_path,
                                                const gchar *interface_name,
                                                const gchar *signal_name,
                                                GVariant *parameters,
                                                gpointer user_data) {
    PlayerProvider *_this = ((PlayerProvider *) user_data);
    GVariantIter iter;
    GVariant *child;

    std::cout << "Caught signal: " << signal_name << std::endl;
    GVariant *inner = g_variant_get_child_value(parameters, 1);

    g_variant_iter_init (&iter, inner);
    while ((child = g_variant_iter_next_value (&iter)))
      {
        if (g_variant_is_of_type(child, G_VARIANT_TYPE_STRING)) {
        } if (g_variant_is_of_type (child, G_VARIANT_TYPE_DICT_ENTRY)) {
            GVariant *key = g_variant_get_child_value(child, 0);
            GVariant *value = g_variant_get_variant(g_variant_get_child_value(child, 1));
            std::string keyStr = std::string(g_variant_get_string (key, NULL));
            _this->handlePropertyChangedSignal (keyStr, value);

            g_variant_unref (key);
            g_variant_unref (value);
        }

        g_variant_unref (child);
      }
}

void PlayerProvider::handlePropertyChangedSignal (std::string key, GVariant *value) {
    if (!this->stub) {
            std::cout << "Can't send notification: No stub" << std::endl;
    }

    if (key == "PlaybackStatus") {
        std::cout << "Key: " << key << std::endl;
        std::cout << "Value: " << g_variant_get_string (value, NULL) << std::endl;
        if (g_strcmp0(g_variant_get_string(value,NULL), "Playing") == 0)
            this->stub->setPlaybackStatusAttribute(org::genivi::MediaManager::Player::PlaybackStatus::PLAYING);
        else if (g_strcmp0(g_variant_get_string(value,NULL), "Paused") == 0)
            this->stub->setPlaybackStatusAttribute(org::genivi::MediaManager::Player::PlaybackStatus::PAUSED);
        else if (g_strcmp0(g_variant_get_string(value,NULL), "Stopped") == 0) {
            next(NULL);
        } else {
            std::cout << "Unhandled playback state" << std::endl;
        }
    } else if (key == "Metadata") {
        std::cout << "Key: " << key << std::endl;
        this->stub->setCurrentTrackAttribute(playQueuePosition);
    } else {
        std::cout << "Unhandled key: " << key << std::endl;
    }
}

bool PlayerProvider::registerSignalListener(std::string objectPath) {
    GError *error = NULL;

    GDBusConnection *connection = g_bus_get_sync (
        G_BUS_TYPE_SESSION,
        NULL,
        &error);

    if (error) {
        std::cout << "Failed to set up connection for signal listener:" << error->message <<std::endl;
        return false;
    }

    if (!m_signalHandlerId) {
        m_signalHandlerId = g_dbus_connection_signal_subscribe (connection,
                                    "com.intel.dleyna-renderer",
                                    "org.freedesktop.DBus.Properties",
                                    "PropertiesChanged",
                                    NULL,
                                    NULL,
                                    G_DBUS_SIGNAL_FLAGS_NONE,
                                    playerprovider_dbus_properties_changed_cb,
                                    this,
                                    NULL);
    }

    return true;
}

/*TODO: Move to some common place */
bool PlayerProvider::connectMediaContainer (const std::string path,
                                             dleynaServerMediaContainer2 **mc,
                                             MmError **e) {
    GError *error = NULL;

    if (!g_variant_is_object_path (path.c_str())) {
        std::string error = "Path is invalid";
        std::cout << error << std::endl;
        if (e)
            (*e)->message = error;
        return false;
    }


    *mc = dleyna_server_media_container2_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-server",
                                    path.c_str(),
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating MediaContainer2 proxy: " << error->message << std::endl;
        if (e)
            (*e)->message = error->message;
        return false;
    }

    return true;
}

std::string PlayerProvider::getDisplayName (json_t *item, bool &ok) {
    if (!item) {
        std::cout << "Item is null" << std::endl;
        ok = false;
        return "";
    }

    json_t *displayName = json_object_get (item, "DisplayName");
    if (!displayName) {
        std::cout << "Key 'DisplayName' not found" << std::endl;
        ok = false;
        return "";
    }

    const char *displayNameStr = json_string_value(displayName);
    if (!displayNameStr) {
        std::cout << "DisplayName is not a string" << std::endl;
        ok = false;
        return "";
    }

    ok = true;
    return std::string(displayNameStr);
}

std::string PlayerProvider::getLocalURL (json_t *item, bool &ok) {
    if (!item) {
        std::cout << "Item is null" << std::endl;
        ok = false;
        return "";
    }

    json_t *resources = json_object_get (item, "Resources");
    if (!resources) {
        std::cout << "Key 'Resources' not found" << std::endl;
        ok = false;
        return "";
    }

    for (int i = 0; i < json_array_size (resources); i++) {
        json_t *url = json_object_get (json_array_get(resources, i), "URL");
        if (!url) {
            std::cout << "Unable to find URL property in Resource" << std::endl;
            ok = false;
            return "";
        }

        const char *urlStr = json_string_value(url);
        if (!urlStr) {
            std::cout << "URL is not a string" << std::endl;
            ok = false;
            return "";
        }
        if (g_str_has_prefix (urlStr, "file://")) {
            std::cout << "Found local URL: " << urlStr << std::endl;
            ok = true;
            return urlStr;
        }
    }

    ok = false;
    return "";
}

void PlayerProvider::next(MmError **e) {
    uint newPosition = 0;

    if (m_shuffle) {
        int playQueueSize = json_array_size (playqueue) - 1;
        newPosition = g_random_int_range (-playQueuePosition, playQueueSize - playQueuePosition);
    }

    if (!changePlayQueuePosition(newPosition, e)) {
        pause(e);
    }
}

void PlayerProvider::previous(MmError **e) {
    changePlayQueuePosition(-1, e);
}

void PlayerProvider::setRate (double rate, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                           *error = NULL;
    dleynaRendererMediaPlayer2Player *mp    = NULL;

    if (!mp && !PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_set_rate (mp, rate);

    checkError (error, e);
}

void PlayerProvider::setRepeat (bool repeat) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    m_repeat = repeat;
}

void PlayerProvider::setShuffle (bool repeat) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    m_shuffle = repeat;
}

bool PlayerProvider::changePlayQueuePosition (int increment, MmError **e) {
    GError                           *error = NULL;
    dleynaRendererMediaPlayer2Player *mp    = NULL;

    if (playqueue) {
        json_t *currentItem = NULL;
        int playQueueSize = json_array_size (playqueue) - 1;
        bool ok;
        std::string displayName, localURL;
        int newPlayQueuePosition;

        newPlayQueuePosition = playQueuePosition + increment;

        if (playQueueSize < newPlayQueuePosition) {
            std::cout << "Play queue is smaller than " << playQueuePosition << " elements" << std::endl;
            if (m_repeat && playQueueSize >= 0) {
                std::cout << "Repeat is enabled, resetting play queue position to 0" << std::endl;
                newPlayQueuePosition = 0;
            } else {
                std::cout << "Repeat disabled. Pausiong playback" << std::endl;
                return false;
            }
        }

        if (newPlayQueuePosition < 0) {
            std::cout << "Illegal play queue position: " << playQueuePosition << std::endl;
            return false;
        }

        playQueuePosition = newPlayQueuePosition;

        this->stub->setCanGoNextAttribute(playQueueSize >= newPlayQueuePosition + 1);
        this->stub->setCanGoPreviousAttribute(newPlayQueuePosition - 1 >= 0);

        std::cout << "Playlist contains " << playQueueSize << " items" << std::endl;
        currentItem = json_array_get(playqueue, playQueuePosition);

        displayName = getDisplayName(currentItem, ok);
        if (ok)
            std::cout << "Current item is: " << displayName << std::endl;
        else
            std::cout << "Failed to get DisplayName for current item" << std::endl;

        localURL = getLocalURL(currentItem, ok);
        if (ok)
            std::cout << "Local URL for item is: " << localURL << std::endl;
        else
            std::cout << "Failed to get localURL for current item" << std::endl;

        openURI (localURL, e);
        return true;

    } else {
        std::cout << "No play queue has been loaded";
        return false;
    }

    return false;
}
