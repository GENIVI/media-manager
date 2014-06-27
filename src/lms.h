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
#include <functional>

#include <gio/gio.h>

#include "common.h"

class LMSProvider {

public:
    LMSProvider() : 
        connection(NULL) {}
    ~LMSProvider();
    void getDatabasePath (std::string &database_path, MmError **e);
    void startIndexing ();
    void stopIndexing ();
    std::string getIndexerStatus ();

    bool connect(std::function<void(MmError *e)>);
    bool isConnected();

    void disconnect();

    unsigned int m_watcherId;
    GDBusConnection *connection;

};

#endif /* LMS_H */
