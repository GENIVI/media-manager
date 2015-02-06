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

#include "playerstub.h"

namespace MM = org::genivi::mediamanager;

PlayerStubImpl::PlayerStubImpl (PlayerProvider *player) {
    m_player = player;
    m_player->stub = this;

    initializeDefaultValues();

    m_generalFilter.push_back("Path");
    m_generalFilter.push_back("Parent");
    m_generalFilter.push_back("Type");
    m_generalFilter.push_back("TypeEx");
    m_generalFilter.push_back("DisplayName");
    m_generalFilter.push_back("ChildCount");
    m_generalFilter.push_back("Artists");
    m_generalFilter.push_back("Artist");
    m_generalFilter.push_back("Searchable");
    m_generalFilter.push_back("MIMEType");
    m_generalFilter.push_back("Album");
    m_generalFilter.push_back("URI");
    m_generalFilter.push_back("Genre");
    m_generalFilter.push_back("Size");
    m_generalFilter.push_back("TrackNumber");
    m_generalFilter.push_back("Bitrate");
    m_generalFilter.push_back("SampleRate");
    m_generalFilter.push_back("Duration");
    m_generalFilter.push_back("AlbumArtURL");
}

void PlayerStubImpl::initializeDefaultValues() {
    /* Mute */
    if (m_player->m_muted)
        trySetMuteAttribute (MM::PlayerTypes::MuteStatus::MUTED);
    else
        trySetMuteAttribute (MM::PlayerTypes::MuteStatus::UNMUTED);

    /* Shuffle */
    if (m_player->m_shuffle)
        trySetShuffleAttribute(MM::PlayerTypes::ShuffleStatus::SHUFFLE);
    else
       trySetShuffleAttribute(MM::PlayerTypes::ShuffleStatus::UNSHUFFLE);

    /* Repeat */
    trySetRepeatAttribute (m_player->m_repeat);

    /* Rate */
    trySetRateAttribute(m_player->m_playrate);

    /* Volume */
    trySetVolumeAttribute (m_player->m_volume);

    /* Go next */
    trySetCanGoNextAttribute(false);

    /* Go previous */
    trySetCanGoPreviousAttribute(false);

    /* Can Pause */
    trySetCanPauseAttribute (false);

    /* Can Play */
    trySetCanPlayAttribute (false);

    /* Can Seek */
    trySetCanSeekAttribute (false);

    /* Current track */
    trySetCurrentTrackAttribute (m_player->playQueuePosition);

    /* Playback status */
    if (m_player->isPlaying)
        trySetPlaybackStatusAttribute (MM::PlayerTypes::PlaybackStatus::PLAYING);
    else
        trySetPlaybackStatusAttribute (MM::PlayerTypes::PlaybackStatus::PAUSED);

    /* Play position */
    trySetPositionAttribute (UINT64_MAX - 1);
}

void PlayerStubImpl::next(MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError("");

    m_player->next(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::openUri(std::string uri, MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError("");

    m_player->openURI(uri, &error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::openPlaylist(std::string uri, MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->openPlaylist(uri, &error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::pause(MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->pause(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::play(MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->play(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::playPause(MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->playPause(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::previous(MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");

    m_player->previous(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::seek(int64_t pos, MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->seek(pos, &error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::setPosition(uint64_t pos, MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->setPosition(pos, &error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::stop(MM::PlayerTypes::PlayerError& e){
    MmError *error = new MmError("");
    m_player->stop(&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::enqueueUri(std::string uri, MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError();
    m_player->enqueueUri (uri, &error);

    if (error) {
        std::cout << "Error in " << __FUNCTION__ << " " << error->message << std::endl;
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::dequeueIndex(uint64_t pos, MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError("");
    m_player->dequeueIndex (pos, &error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::dequeueAll(MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError("");
    m_player->dequeueAll (&error);

    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::getCurrentPlayQueue(MM::MediaTypes::ResultMapList& playQueue, MM::PlayerTypes::PlayerError& e) {
    MmError *error = new MmError("");
    json_t* queue;
    m_player->getCurrentPlayQueue (&queue, &error);

    Common::resultMapListToCAPIResultMapList(queue, playQueue, m_generalFilter);
    if (error) {
        e = MM::PlayerTypes::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

const uint64_t& PlayerStubImpl::getPositionAttribute(const std::shared_ptr<CommonAPI::ClientId> clientId) {
    pos = m_player->getPosition(NULL);
    return pos;
}

const uint64_t& PlayerStubImpl::getDurationAttribute(const std::shared_ptr<CommonAPI::ClientId> clientId) {
    duration = m_player->getDuration(NULL);
    return duration;
}

void PlayerStubImpl::onRemoteRateAttributeChanged() {
    std::cout << "Remote has updated Rate attribute" << std::endl;
    double rate = getRateAttribute();

    if (rate < -16 || rate > 16) {
        std::cout << "Bad rate selected" << std::endl;
    }

    m_player->setRate (rate, NULL);
}

void PlayerStubImpl::onRemoteRepeatAttributeChanged() {
    std::cout << "Remote has updated repeat attribute" << std::endl;
    m_player->setRepeat(getRepeatAttribute());
}
void PlayerStubImpl::onRemoteShuffleAttributeChanged() {
    std::cout << "Remote has updated shuffle attribute" << std::endl;

    if (getShuffleAttribute() == MM::PlayerTypes::ShuffleStatus::SHUFFLE)
        m_player->setShuffle(true);
    else
        m_player->setShuffle(false);
}
