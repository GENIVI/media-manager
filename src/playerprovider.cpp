 /*
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

PlayerProvider::PlayerProvider() :
    ServiceProvider("com.intel.dleyna-renderer"),
    playQueuePosition(UINT64_MAX),
    stub(0),
    mp(0),
    mc(0),
    playqueue(0),
    isPlaying(0),
    m_signalHandlerId(0),
    m_shuffle(0),
    m_muted(0),
    m_playrate(1),
    m_repeat (org::genivi::mediamanager::PlayerTypes::RepeatStatus::REPEAT),
    m_volume(1)
{}
char *PlayerProvider::findFirstPlayer(MmError **e) {
    std::vector<std::string> renderers = Common::discoverDLNABackends ("renderers", e);
    if (renderers.size() == 0) {
        std::cout << "No renderers found!" << std::endl;
        return NULL;
    }

    std::string renderer = renderers.front();
    return strdup (renderer.c_str());
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
        this->stub->setPlaybackStatusAttribute(org::genivi::mediamanager::PlayerTypes::PlaybackStatus::PAUSED);
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
    int count = 100000;
    gchar **filterStrv = Common::stdStrvToStrv(filter);

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
    playqueue = Common::DLNADictToJSON (out);

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
        if (e && *e)
            (*e)->message = error;
        return false;
    }

    if (*mp)
        g_object_unref (*mp);

    *mp = dleyna_renderer_media_player2_player_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-renderer",
                                    foundpath,
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating MediaPlayer2 proxy: " << error->message << std::endl;
        if (e && *e)
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
        if (g_strcmp0(g_variant_get_string(value,NULL), "Playing") == 0)
            this->stub->setPlaybackStatusAttribute(org::genivi::mediamanager::PlayerTypes::PlaybackStatus::PLAYING);
        else if (g_strcmp0(g_variant_get_string(value,NULL), "Paused") == 0)
            this->stub->setPlaybackStatusAttribute(org::genivi::mediamanager::PlayerTypes::PlaybackStatus::PAUSED);
        else if (g_strcmp0(g_variant_get_string(value,NULL), "Stopped") == 0) {
            next(NULL);
        } else {
            std::cout << "Unhandled playback state" << std::endl;
        }
    } else if (key == "Metadata") {
        this->stub->setCurrentTrackAttribute(playQueuePosition);
    } else if (key == "CanPlay") {
        this->stub->setCanPlayAttribute(g_variant_get_boolean (value));
    } else if (key == "CanPause") {
        this->stub->setCanPauseAttribute(g_variant_get_boolean (value));
    } else if (key == "CanSeek") {
        this->stub->setCanSeekAttribute(g_variant_get_boolean (value));
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
        if (e && *e)
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
        if (e && *e)
            (*e)->message = error->message;
        return false;
    }

    return true;
}

bool PlayerProvider::connectMediaObject (const std::string path,
                                             dleynaServerMediaObject2 **mo,
                                             MmError **e) {
    GError *error = NULL;

    if (!g_variant_is_object_path (path.c_str())) {
        std::string error = "Path is invalid";
        std::cout << error << std::endl;
        if (e && *e)
            (*e)->message = error;
        return false;
    }


    *mo = dleyna_server_media_object2_proxy_new_for_bus_sync (
                                    G_BUS_TYPE_SESSION,
                                    G_DBUS_PROXY_FLAGS_NONE,
                                    "com.intel.dleyna-server",
                                    path.c_str(),
                                    NULL,
                                    &error);
    if (error) {
        std::cout << "Error creating MediaContainer2 proxy: " << error->message << std::endl;
        if (e && *e)
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
    uint newPosition = 1;

    if (m_repeat == org::genivi::mediamanager::PlayerTypes::RepeatStatus::REPEAT_SINGLE) {
        newPosition = 0;
    }

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

    m_playrate = rate;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_set_rate (mp, rate);

    checkError (error, e);
}

void PlayerProvider::setRepeat (org::genivi::mediamanager::PlayerTypes::RepeatStatus repeat) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    m_repeat = repeat;
}

void PlayerProvider::setShuffle (bool repeat) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    m_shuffle = repeat;
}

void PlayerProvider::stop (MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    pause(e);
    seek(0, e);
}

void PlayerProvider::seek (uint64_t pos, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                           *error = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    dleyna_renderer_media_player2_player_call_seek_sync (mp,
                                                         pos,
                                                         NULL,
                                                         &error);

    checkError (error, e);
}

void PlayerProvider::setPosition (uint64_t pos, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                           *error = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return;
    gint64 currentPosition = dleyna_renderer_media_player2_player_get_position (mp);

    seek (pos - currentPosition, e);

    checkError (error, e);
}

uint64_t PlayerProvider::getPosition (MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                           *error = NULL;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return 0;
    gint64 currentPosition = dleyna_renderer_media_player2_player_get_position (mp);

    checkError (error, e);

    if (currentPosition >= 0)
        return currentPosition;
    return 0;
}

uint64_t PlayerProvider::getDuration (MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError *error = NULL;
    GVariant *duration = NULL;
    GVariant *metadata = NULL;
    uint64_t currentDuration = 0;

    if (!PlayerProvider::connectMediaPlayer(PLAYER_PATH, &mp, e))
        return 0;

    metadata = dleyna_renderer_media_player2_player_get_metadata (mp);

    duration = g_variant_lookup_value (metadata, "mpris:length", G_VARIANT_TYPE_INT64);

    if (duration != NULL) {
        int64_t d = g_variant_get_int64(duration);
        if (d > 0)
            currentDuration = d;
    } else {
        std::cout << "mpris:length not a member of Metadata struct" << std::endl;
    }

    checkError (error, e);

    return currentDuration;
}

void PlayerProvider::enqueueUri (std::string uri, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    GError                          *error = NULL;
    std::vector<std::string>         filter;
    GVariant                        *out    = NULL;

    filter.push_back("*");

    gchar **filterStrv = Common::stdStrvToStrv(filter);

    if (!PlayerProvider::connectMediaObject(uri, &mo, e))
        return;

    if (!playqueue) {
        std::cout << "Initializing new play queue" << std::endl;
        playqueue = json_array();
    }

    const char *parent = dleyna_server_media_object2_get_parent (mo);
    if (!parent) {
        std::cout << "Can't get parent, URI stale?" << std::endl;
        return;
    }

    if (!connectMediaContainer(parent, &mc, e))
        return;

    dleyna_server_media_container2_call_list_items_sync (mc, 0, 100,
                                                         filterStrv, &out, NULL,
                                                         &error);
    if (error) {
        std::cout << "Error listing items in " << parent << std::endl;
        return;
    }

    GVariantIter iter;
    GVariant *child;

    g_variant_iter_init (&iter, out);
    while ((child = g_variant_iter_next_value (&iter))) {
        GVariant *ref = g_variant_lookup_value (child, "Path", G_VARIANT_TYPE_OBJECT_PATH);
        if (ref) {
            if (g_variant_get_string(ref, NULL) == uri) {
                std::cout << "Enqueueing " << uri << std::endl;

                json_t *js = Common::DLNADictToJSON(child);
                json_array_append(playqueue, js);
            }
        }
        g_variant_unref (child);
      }

    checkError (error, e);
}

void PlayerProvider::dequeueIndex (uint64_t idx, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
}

void PlayerProvider::dequeueAll (MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    json_array_clear(playqueue);
}

void PlayerProvider::getCurrentPlayQueue (Common::ResultMapList **queue, MmError **e) {
    std::cout << "In function: " << __FUNCTION__ << std::endl;
    json_t *playQueueCopy = json_copy(playqueue);
    Common::postProcessJSON(playQueueCopy);
    *queue = playQueueCopy;
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
            if (m_repeat == org::genivi::mediamanager::PlayerTypes::RepeatStatus::REPEAT && playQueueSize >= 0) {
                std::cout << "Repeat is enabled, resetting play queue position to 0" << std::endl;
                newPlayQueuePosition = 0;
            } else {
                std::cout << "Repeat disabled. Pausing playback" << std::endl;
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
