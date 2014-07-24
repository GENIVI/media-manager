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

cmake_minimum_required(VERSION 2.6)

INCLUDE (../common.cmake)

SET (SOURCES
    lms.cpp
    main.cpp
    indexerstub.cpp
    serviceprovider.cpp
    browserprovider.cpp
    playerprovider.cpp
    browserstub.cpp
    playerstub.cpp)

SET (HEADERS
    lms.h
    indexerstub.h
    serviceprovider.h
    browserprovider.h
    playerprovider.h
    browserstub.h
    browserstub.h)

ADD_CUSTOM_COMMAND (OUTPUT ${GENERATED_SOURCES_CAPI} ${GENERATED_HEADERS_CAPI}
                    COMMAND git submodule init &&
                            git submodule update)

ADD_EXECUTABLE (media-manager
                ${SOURCES}
                ${HEADERS}
                ${GENERATED_SOURCES_CAPI}
                ${GENERATED_HEADERS_CAPI}
                ${GENERATED_SOURCES_DBUS})

TARGET_LINK_LIBRARIES (media-manager ${PC_GIO_LIBRARIES}
                                     ${PC_COMMONAPI_LIBRARIES}
                                     ${PC_COMMONAPIDBUS_LIBRARIES}
                                     ${PC_GIO_UNIX_LIBRARIES}
                                     ${PC_JANSSON_LIBRARIES})

INSTALL(TARGETS media-manager
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
