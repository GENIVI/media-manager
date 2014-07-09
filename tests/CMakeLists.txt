# Copyright (C) 2014, Jaguar Land Rover
#
# Author: Jonatan Palsson <jonatan.palsson@pelagicore.com>
#
# This file is part of the GENIVI Media Manager Proof-of-Concept
# For further information, see http://genivi.org/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_SUBDIRECTORY (gmock-1.7.0)
SET (GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/tests/gmock-1.7.0/include")
SET (GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/tests/gmock-1.7.0/gtest/include")

ADD_SUBDIRECTORY (lmsprovider)
ADD_SUBDIRECTORY (browserprovider)
