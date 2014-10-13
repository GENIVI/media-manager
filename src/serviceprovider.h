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

#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <functional>

#include <gio/gio.h>

#include "common.h"

/**
 * Common base class for service providers connecting to services over D-Bus.
 */
class ServiceProvider {

public:
    ServiceProvider(std::string);
    ~ServiceProvider();

    /**
     * Connect to the D-Bus service of the backend service
     *
     * @param cb Callback to use for reporting success or failure when
     *           connecting
     */
    bool connect(std::function<void(MmError *e)> cb);

    /**
     * Determine is service provider is connected to backend service
     *
     * @return true if we're connected
     * @return false if we're not connected
     */
    bool isConnected();

    /**
     * Check if GError object is set. Convert to MmError if set.
     *
     * @param gerror GError to check
     * @param mmerror MmError to set of gerror is non-null
     * @return true if mmerror is set
     * @return false if mmerror is not set
     */
    bool checkError (GError * gerror, MmError** mmerror);

    /**
     * Disconnect from backend service
     */
    void disconnect();

    unsigned int m_watcherId;
    GDBusConnection *connection;
    std::string ifacePath;

    std::function<void(MmError *)> *onConnectedCallback;
};

#endif /* SERVICEPROVIDER_H */
