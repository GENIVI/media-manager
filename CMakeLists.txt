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
PROJECT (MEDIAMANAGER)

ADD_SUBDIRECTORY (src)
#ADD_SUBDIRECTORY (tests)

INSTALL (FILES 10-media-manager-usb-mount-rules.rules
         DESTINATION /etc/udev/rules.d/)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
               "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
               @ONLY)

ADD_CUSTOM_TARGET(doc
              doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
              COMMENT "Generating API documentation with Doxygen" VERBATIM)
