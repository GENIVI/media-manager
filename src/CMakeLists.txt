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

SET (CUSTOM_DBUS_PATH /usr/local/lib/)
SET (INTERFACE_DIR ${MEDIAMANAGER_SOURCE_DIR}/interfaces/src-gen/)

FIND_PACKAGE (PkgConfig)

pkg_check_modules (PC_GIO REQUIRED gio-2.0)
pkg_check_modules (PC_COMMONAPI REQUIRED CommonAPI)
pkg_check_modules (PC_COMMONAPIDBUS REQUIRED CommonAPI-DBus)

INCLUDE_DIRECTORIES (${PC_GIO_INCLUDE_DIRS}
                     ${PC_COMMONAPI_INCLUDE_DIRS}
                     ${PC_COMMONAPIDBUS_INCLUDE_DIRS})

INCLUDE_DIRECTORIES (${INTERFACE_DIR})

LINK_DIRECTORIES (${CUSTOM_DBUS_PATH})

ADD_DEFINITIONS (${PC_COMMONAPIDBUS_CFLAGS})

SET(CMAKE_CXX_FLAGS "-std=c++0x")

SET (GENERATED_SOURCES
    ${INTERFACE_DIR}/org/genivi/MediaManager/Browser.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserDBusProxy.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserDBusStubAdapter.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserStubDefault.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/Indexer.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerDBusProxy.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerDBusStubAdapter.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerStubDefault.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/Player.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerDBusProxy.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerDBusStubAdapter.cpp
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerStubDefault.cpp
)

SET (GENERATED_HEADERS
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserDBusProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserDBusStubAdapter.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/Browser.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserProxyBase.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserServiceAbstract.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserStubDefault.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/BrowserStub.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerDBusProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerDBusStubAdapter.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/Indexer.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerProxyBase.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerServiceAbstract.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerStubDefault.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/IndexerStub.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerDBusProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerDBusStubAdapter.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/Player.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerProxyBase.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerProxy.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerServiceAbstract.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerStubDefault.h
    ${INTERFACE_DIR}/org/genivi/MediaManager/PlayerStub.h
)

SET (SOURCES
    lms.cpp
    main.cpp
    indexerstub.cpp)

SET (HEADERS
    lms.h
    indexerstub.h)

ADD_CUSTOM_COMMAND (OUTPUT ${GENERATED_SOURCES} ${GENERATED_HEADERS}
                    COMMAND git submodule init &&
                            git submodule update)

ADD_EXECUTABLE (media-manager
                ${SOURCES}
                ${HEADERS}
                ${GENERATED_SOURCES}
                ${GENERATED_HEADERS})

TARGET_LINK_LIBRARIES (media-manager ${PC_GIO_LIBRARIES}
                                     ${PC_COMMONAPI_LIBRARIES}
                                     ${PC_COMMONAPIDBUS_LIBRARIES})
