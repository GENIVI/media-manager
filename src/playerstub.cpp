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
}

void PlayerStubImpl::next(MM::Player::PlayerError& e) {
    std::cout << __FUNCTION__ << " is not implemented" << std::endl;
}

void PlayerStubImpl::openUri(std::string uri, MM::Player::PlayerError& e) {
    MmError *error = NULL;

    m_player->openURI(uri, &error);

    if (error) {
        e = MM::Player::PlayerError::BACKEND_UNREACHABLE;
    }
}

void PlayerStubImpl::openPlaylist(std::string uri, MM::Player::PlayerError& e){
    std::cout << __FUNCTION__ << " is not implemented" << std::endl;
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
    std::cout << __FUNCTION__ << " is not implemented" << std::endl;
}

void PlayerStubImpl::seek(int64_t pos, MM::Player::PlayerError& e){
    std::cout << __FUNCTION__ << " is not implemented" << std::endl;
}

void PlayerStubImpl::setPosition(uint64_t pos, MM::Player::PlayerError& e){
    std::cout << __FUNCTION__ << " is not implemented" << std::endl;
}

const MM::Player::MuteStatus& PlayerStubImpl::getMuteAttribute(const std::shared_ptr<CommonAPI::ClientId> clientId) {
    std::cout << "In " << __FUNCTION__ << std::endl;
    return MM::Player::MuteStatus::MUTED;
}
