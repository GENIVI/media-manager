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

#include <iostream>
#include "serviceprovider.h"

ServiceProvider::ServiceProvider (std::string iface) :
    connection (NULL),
    onConnectedCallback (NULL)
{
    ifacePath = iface;
}

static void onNameAppeared (GDBusConnection *connection,
                     const gchar     *name,
                     const gchar     *name_owner,
                     void            *user_data)
{
    ServiceProvider *this_ = ((ServiceProvider *) user_data);
    std::cout << "Found " << 
        ((ServiceProvider *)user_data)->ifacePath << " on D-Bus" << std::endl;
    ((ServiceProvider *) user_data)->connection = connection;
    (*(this_->onConnectedCallback)) (NULL);
}

static void onNameVanished (GDBusConnection *connection,
                     const gchar     *name,
                     void        *user_data)
{
    ServiceProvider *this_ = ((ServiceProvider *) user_data);
    g_printerr ("Failed to get name owner for %s\n",
                name);
    (*(this_->onConnectedCallback)) (new MmError("Failed to connect, is " +
        this_->ifacePath + " running?"));
}

bool ServiceProvider::connect(std::function<void(MmError *e)> cb)
{
    onConnectedCallback = new std::function<void(MmError *e)> (cb);
    GError *error = NULL;

    /* Create a dummy proxy, so that we can poke the service to life if it
       is activatable, but not yet started. Then use the g_bus_watcher for
       further interaction. DBus activation is not supported using the
       watcher. */
    GDBusConnection *conn = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                            NULL,
                                            &error);

    if (error) {
        g_printerr ("Failed to get bus: %s", error->message);
        return false;
    }

    GDBusProxy *proxy = g_dbus_proxy_new_sync (conn,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               ifacePath.c_str(),
                                               "/",
                                               "org.freedesktop.DBus.Peer",
                                               NULL,
                                               &error);
   g_object_unref(proxy);
   g_object_unref(conn);


    uint m_watcherId = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                   ifacePath.c_str(),
                                   G_BUS_NAME_WATCHER_FLAGS_NONE,
                                   onNameAppeared,
                                   onNameVanished,
                                   this,
                                   NULL);
    return true;
}

bool ServiceProvider::isConnected() {
    return (connection && 
            !g_dbus_connection_is_closed (connection));
}

void ServiceProvider::disconnect() {
    //g_bus_unwatch_name (m_watcherId);
}

ServiceProvider::~ServiceProvider() {
    disconnect();
}

bool ServiceProvider::checkError (GError *error, MmError **e) {
    if (error) {
        std::cout << "Error in " << __FUNCTION__ << " D-Bus call: " << error->message << std::endl;
        if (e)
            (*e) = new MmError(error->message);
        return false;
    }
    return true;
}
