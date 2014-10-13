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

#ifndef LMS_H
#define LMS_H

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <functional>

#include <gio/gio.h>

#include "common.h"

/** \brief Provides the indexing functionality
 *
 *  This class provides all indexing functionality. Typically this means this
 *  class maintains connections to some backing service (for instance LMS, as
 *  in the Media Manager PoC), and will call the appropriate functions in the
 *  backing service when functionality is called in this class.
 */
class LMSProvider {

public:
    LMSProvider() :
        connection(NULL) {}
    ~LMSProvider();
    /** @description Get the path to the database file in the host file system.
     *                This is the database file used by LMS to store metadata
     *  @param database_path Path to database file in host file system
     *  @param e Set if an error occurs, unchanged on success
     *  @deprecated This is only used for testing purposes and will be removed
     */
    void getDatabasePath (std::string &database_path, MmError **e);

    /** @description If indexer is currently STOPPED (see getIndexerStatus),
     *                status is changed to RUNNING and indexing is RUNNING. If
     *                the status is IDLE or RUNNING, no action is taken.
     *                Note that the status may go from RUNNING to IDLE directly
     *                after issuing this call, if no files are to be indexed
     *  @param e Set if an error occurs, unchanged on success
     */
    void startIndexing (MmError **e);

    /** @description: If the indexer is currently IDLE or RUNNING
     *                (see getIndexerStatus), stop indexing and set IndexerStatus
     *                to STOPPED.
     *                If indexer is currently STOPPED, no action is taken
     *  @param e Set if an error occurs, unchanged on success
     */
    void stopIndexing (MmError **e);

    /** @description: RUNNING: The indexer is currently traversing files and/or
     *                         extracting metadata
     *                STOPPED: The indexer has been stopped and will not start
     *                           indexing by itself
     *                IDLE:    The indexer has finished, and is waiting for new
     *                           files to index
     *  @param e Set if an error occurs, unchanged on success
     */
    std::string getIndexerStatus (MmError **e);

    /**
     * Connect to the running LMS instance over D-Bus
     *
     * @param cb The callback to execute upon success or failure of connection
     */
    bool connect(std::function<void(MmError *e)> cb);

    /**
     * Determine if we are connected to an LMS instance
     * @returns true if connected
     * @returns false if not connected
     */
    bool isConnected();

    /**
     * Disconnect from the LMS instance
     */
    void disconnect();

    unsigned int m_watcherId;
    GDBusConnection *connection;

};

#endif /* LMS_H */
