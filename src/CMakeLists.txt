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

SET (GENERATED_SOURCES_CAPI
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

SET (GENERATED_HEADERS_CAPI
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

SET (GENERATED_SOURCES_DBUS
    ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.c
    ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.h
    ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.c
    ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.h
    )

SET (SOURCES
    lms.cpp
    main.cpp
    indexerstub.cpp
    serviceprovider.cpp
    browserprovider.cpp
    browserstub.cpp)

SET (HEADERS
    lms.h
    indexerstub.h
    serviceprovider.h
    browserprovider.h
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
