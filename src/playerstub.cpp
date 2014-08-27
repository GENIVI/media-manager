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

#include "playerstub.h"

namespace MM = org::genivi::MediaManager;

PlayerStubImpl::PlayerStubImpl (PlayerProvider *player) {
    m_player = player;
    m_player->stub = this;

    initializeDefaultValues();
}

void PlayerStubImpl::initializeDefaultValues() {
    /* Mute */
    if (m_player->m_muted)
        trySetMuteAttribute (MM::Player::MuteStatus::MUTED);
    else
        trySetMuteAttribute (MM::Player::MuteStatus::UNMUTED);

    /* Shuffle */
    if (m_player->m_shuffle)
        trySetShuffleAttribute(MM::Player::ShuffleStatus::SHUFFLE);
    else
       trySetShuffleAttribute(MM::Player::ShuffleStatus::UNSHUFFLE);

    /* Repeat */
    setRepeatAttribute (m_player->m_repeat);

    /* Rate */
    switch ((int) m_player->m_playrate) {
        case 1:
            trySetRateAttribute(MM::Player::RateStatus::RATE_1);
            break;
        case 2:
            trySetRateAttribute(MM::Player::RateStatus::RATE_2);
            break;
        case 4:
            trySetRateAttribute(MM::Player::RateStatus::RATE_4);
            break;
        case 8:
            trySetRateAttribute(MM::Player::RateStatus::RATE_8);
            break;
        case 16:
            trySetRateAttribute(MM::Player::RateStatus::RATE_16);
            break;
    }

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
        trySetPlaybackStatusAttribute (MM::Player::PlaybackStatus::PLAYING);
    else
        trySetPlaybackStatusAttribute (MM::Player::PlaybackStatus::PAUSED);

    /* Play position */
    trySetPositionAttribute (UINT64_MAX - 1);
}

void PlayerStubImpl::next(MM::Player::PlayerError& e) {
    MmError *error = NULL;

    m_player->next(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
    }
}

void PlayerStubImpl::openUri(std::string uri, MM::Player::PlayerError& e) {
    MmError *error = NULL;

    m_player->openURI(uri, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
    }
}

void PlayerStubImpl::openPlaylist(std::string uri, MM::Player::PlayerError& e){
    MmError *error = new MmError("");
    m_player->openPlaylist(uri, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::pause(MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->pause(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::play(MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->play(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::playPause(MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->playPause(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::previous(MM::Player::PlayerError& e){
    MmError *error = NULL;

    m_player->previous(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
    }
}

void PlayerStubImpl::seek(int64_t pos, MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->seek(pos, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::setPosition(uint64_t pos, MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->setPosition(pos, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::stop(MM::Player::PlayerError& e){
    MmError *error = NULL;
    m_player->stop(&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::enqueueUri(std::string uri, MM::Player::PlayerError& e) {
    MmError *error = NULL;
    m_player->enqueueUri (uri, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::dequeueIndex(uint64_t pos, MM::Player::PlayerError& e) {
    MmError *error = NULL;
    m_player->dequeueIndex (pos, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::dequeueAll(MM::Player::PlayerError& e) {
    MmError *error = NULL;
    m_player->dequeueAll (&error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

void PlayerStubImpl::getCurrentPlayQueue(std::string& playQueue, MM::Player::PlayerError& e) {
    MmError *error = NULL;
    std::string queue;
    m_player->getCurrentPlayQueue (playQueue, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
        free (error);
    }
}

const uint64_t& PlayerStubImpl::getPositionAttribute(const std::shared_ptr<CommonAPI::ClientId> clientId) {
    pos = m_player->getPosition(NULL);
    return pos;
}

void PlayerStubImpl::onRemoteRateAttributeChanged() {
    std::cout << "Remote has updated Rate attribute" << std::endl;
    MM::Player::RateStatus rate = getRateAttribute();
    int rateInt = 1;

    switch (rate) {
        case MM::Player::RateStatus::RATE_NEG_16:
            rateInt = -16;
            break;
        case MM::Player::RateStatus::RATE_NEG_8:
            rateInt = -8;
            break;
        case MM::Player::RateStatus::RATE_NEG_4:
            rateInt = -4;
            break;
        case MM::Player::RateStatus::RATE_NEG_2:
            rateInt = -2;
            break;
        case MM::Player::RateStatus::RATE_NEG_1:
            rateInt = -1;
            break;
        case MM::Player::RateStatus::RATE_1:
            rateInt = 1;
            break;
        case MM::Player::RateStatus::RATE_2:
            rateInt = 2;
            break;
        case MM::Player::RateStatus::RATE_4:
            rateInt = 4;
            break;
        case MM::Player::RateStatus::RATE_8:
            rateInt = 8;
            break;
        case MM::Player::RateStatus::RATE_16:
            rateInt = 16;
            break;
    }

    m_player->setRate (rateInt, NULL);
}

void PlayerStubImpl::onRemoteRepeatAttributeChanged() {
    std::cout << "Remote has updated repeat attribute" << std::endl;
    m_player->setRepeat(getRepeatAttribute());
}
void PlayerStubImpl::onRemoteShuffleAttributeChanged() {
    std::cout << "Remote has updated shuffle attribute" << std::endl;

    if (getShuffleAttribute() == MM::Player::ShuffleStatus::SHUFFLE)
        m_player->setShuffle(true);
    else
        m_player->setShuffle(false);
}
