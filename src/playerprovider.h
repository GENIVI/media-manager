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
#include "../src-gen/org/genivi/mediamanager/PlayerStubDefault.h"

/**
 * @brief Provides Player functionality
 *
 * This class provides Player functionality, and typically maintains a
 * connection to a media player service, or provides a media player in itself.
 * In the Media Manager PoC, this class maintains a connection to dLeyna, which
 * in turn connects to Rygel, which acts as a renderer using GStreamer.
 *
 * Some functionality is directly bridged through to dLeyna, such as
 * play/pause, while other functionality, such as maintaining and manipulating
 * a play queue will manipulate the state of the class implementing this API.
 */
class PlayerProvider : public ServiceProvider {
friend class PlayerStubImpl;

public:
    PlayerProvider();
    ~PlayerProvider() {}

    /** @description Open the supplied Uri for playback in the playback engine.
     *  @param uri URI of media to play, uri format is decided by backend
     *  @param e Any error, unchanged if there is no error
     */
    void openURI(std::string uri, MmError **e);

    /** @description Pause playback in the playback engine.
     *                 If already paused, this has no effect
     *  @param e Any error, unchanged if there is no error
     */
    void pause(MmError **e);
    /** @description: Start or resume playback in the playback engine.
     *                 If playback is already commencing, this has not effect.
     *                 If paused, playback resumes from the current position.
     *                 If there is no track to play, calling this has no effect
     *  @param e Any error, unchanged if there is no error
     */
    void play(MmError **e);

    /** @description: Starts playback (see play) if paused
     *                Pauses playback (see pause) is playing
     *  @see Play
     *  @see Pause
     *  @param e Any error, unchanged if there is no error
     */
    void playPause(MmError **e);

    /** @description Use the supplied playlist as the current play queue. If
     *                the play queue is invalid, the old play queue is
     *                untouched.
     *
     *  @param e Any error, unchanged if there is no error
     */
    void openPlaylist (std::string, MmError **e);

    /** @description: Skip to the next track in the play queue. If there is no
     *                 next track, playback is stopped. If playback is paused or
     *                 stopped, this function will set the next plack to be
     *                 played once playback is activated
     *  @param e Any error, unchanged if there is no error
     */
    void next (MmError **e);

    /** @description: Loads previous track in play queue
     *                If there is no previous track, playback is stopped
     *                If playback is stopped or paused, it remains this way,
     *                but the next track to be played will the set
     *                by this function
     *  @param e Any error, unchanged if there is no error
     */
    void previous (MmError **e);

    /** @description Set the playback rate
     *  @param rate Playback rate to set
     *  @param e Any error, unchanged when no error has occurred
     */

    void setRate (double rate, MmError **e);
    /** @description Toggle repeat for the current playlist. If enabled,
     *                playback will continue with the first item of the current
     *                play queue when the last item has finished playing
     *  @param repeat Repeat mode to set
     */

    void setRepeat (org::genivi::mediamanager::PlayerTypes::RepeatStatus repeat);

    /** @description Shuffle or unshuffle the current play queue. Shuffling,
     *             un-shuffling and then re-shuffling will yield two different
     *             shufflings (which are not guaranteed to be different)
     *  @param shuffle true to enable shuffle, false to disable
     */
    void setShuffle (bool shuffle);

    /** @description Skip to the next track in the play queue. If there is no
     *                 next track, playback is stopped. If playback is paused or
     *                 stopped, this function will set the next plack to be
     *                 played once playback is activated
     *  @param e Any error, unchanged when no error has occurred
     */
    void stop (MmError **e);

    /** @description Jump to the specified position in the current
     *                 If the position is not within range of the current track,
     *                 do nothing
     *  @param pos Absolute position in microseconds
     *  @param e Any error, unchanged when no error has occurred
     */
    void setPosition (uint64_t pos, MmError **e);

   /**
    *  @description Get the position in the current track in microseconds
    *  @param e Any error, unchanged when no error has occurred
    */
    uint64_t getPosition (MmError **e);

   /**
    *  @description Get the duration of the current track in microseconds
    *  @param e Any error, unchanged when no error has occurred
    */
    uint64_t getDuration (MmError **e);

    /** @description Seek relatively in the current track
     *                If a negative seek leads to a play position less than 0,
     *                the play position will be set to 0.
     *  @param pos Relative seek amount in microseconds
     *  @param e Any error, unchanged when no error has occurred
     */
    void seek (uint64_t pos, MmError **e);

    /** @description Enqueue the supplied Uri for playback in the playback
     * engine.
     * @param uri URI of media to enqueue, uri format is decided by backend
     * @param e Any error, unchanged when no error has occurred
     */
    void enqueueUri (std::string uri, MmError **e);

    /** @description Dequeue the item with the supplied index in the playback
     *                engine.
     *  @param pos index of media to dequeue
     *  @param e Any error, unchanged when no error has occurred
     */
    void dequeueIndex (uint64_t idx, MmError **e);

    /** @description: Dequeue all elements, emptying the play queue
     *  @param e Any error, unchanged when no error has occurred
     */
    void dequeueAll (MmError **e);

    /** @description: Retrieve the current play queue in JSON format
     *  @param queue Current play queue in JSON format
     *  @param e Any error, unchanged when no error has occurred
     */
    void getCurrentPlayQueue (std::string &queue, MmError **e);

    /**
     * Callback function to handle the PropertyChanged signal from D-Bus, this
     * is public for technical reasons, and should not be considered part of
     * the API
     */
    void handlePropertyChangedSignal (std::string, GVariant *);

    /**
     * This is a reference to the PlayerStubImpl for this instance
     */
    org::genivi::mediamanager::PlayerStubDefault *stub;
private:
dleynaRendererMediaPlayer2Player *mp;
dleynaServerMediaContainer2      *mc;
dleynaServerMediaObject2         *mo;
uint64_t playQueuePosition;
json_t *playqueue;
bool isPlaying;
guint m_signalHandlerId;
org::genivi::mediamanager::PlayerTypes::RepeatStatus m_repeat;
bool m_shuffle;
bool m_muted;
double m_playrate;
double m_volume;

bool registerSignalListener(std::string);

bool connectMediaPlayer (const std::string path,
                         dleynaRendererMediaPlayer2Player **mc,
                         MmError **e);

bool connectMediaContainer(const std::string path,
                           dleynaServerMediaContainer2 **mc,
                           MmError **e);
bool connectMediaObject (const std::string path,
                         dleynaServerMediaObject2 **mc,
                         MmError **e);

char *findFirstPlayer(MmError **e);

bool changePlayQueuePosition (int increment, MmError **e);

std::string getDisplayName (json_t *item, bool &ok);
std::string getLocalURL (json_t *item, bool &ok);
};

#endif /* PLAYERPROVIDER_H */
