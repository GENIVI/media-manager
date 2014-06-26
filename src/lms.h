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

#ifndef LMS_H
#define LMS_H

#include <string>
#include <unistd.h>
#include <stdlib.h>

#include <gio/gio.h>

class LMSProvider {

public:
    LMSProvider() {}
    ~LMSProvider();
    std::string getDatabasePath ();
    void startIndexing ();
    void stopIndexing ();
    std::string getIndexerStatus ();

    bool connect();
    bool isConnected();

    void disconnect();

    unsigned int m_watcherId;
    GDBusConnection *connection;
};

#endif /* LMS_H */
