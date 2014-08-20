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

    if (getRepeatAttribute() == MM::Player::RepeatStatus::REPEAT)
        m_player->setRepeat(true);
    else
        m_player->setRepeat(false);
}
void PlayerStubImpl::onRemoteShuffleAttributeChanged() {
    std::cout << "Remote has updated shuffle attribute" << std::endl;

    if (getShuffleAttribute() == MM::Player::ShuffleStatus::SHUFFLE)
        m_player->setShuffle(true);
    else
        m_player->setShuffle(false);
}
