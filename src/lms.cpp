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

#include <iostream>

#include "lms.h"
#include "common.h"

std::function<void(MmError *)> *onConnectedCallback = NULL;

void LMSProvider::getDatabasePath (std::string &database_path, MmError **e) {
    GError *error = NULL;
    GVariant *inner, *ret;

    if (!isConnected()) {
        g_warning ("No connection to LMS");
        *e = new MmError("No connection to LMS");
        return;
    }

    ret = g_dbus_connection_call_sync (connection,
                                     "org.lightmediascanner",
                                     "/org/lightmediascanner/Scanner1",
                                     "org.freedesktop.DBus.Properties",
                                     "Get",
                                     g_variant_new ("(ss)",
                                        "org.lightmediascanner.Scanner1",
                                        "DataBasePath"),
                                     G_VARIANT_TYPE_TUPLE,
                                     G_DBUS_CALL_FLAGS_NONE,
                                     -1,
                                     NULL,
                                     &error);
    if (error) {
        if (e && *e)
            (*e) = new MmError(error->message);
        g_error ("%s\n", error->message);
    }

    g_variant_get(ret, "(v)", &inner);
    database_path = std::string(g_variant_get_string(inner, NULL));
    g_variant_unref(ret);
    g_variant_unref(inner);
}

void LMSProvider::startIndexing (MmError **e) {
    GError *error = NULL;
    GVariant *ret;
    GVariant *param, *args;
    GVariantBuilder *b;

    b = g_variant_builder_new (G_VARIANT_TYPE("a{sv}"));
    args = g_variant_builder_end (b);
    param = g_variant_new_tuple(&args,1);

    ret = g_dbus_connection_call_sync (connection,
                                     "org.lightmediascanner",
                                     "/org/lightmediascanner/Scanner1",
                                     "org.lightmediascanner.Scanner1",
                                     "Scan",
                                     param,
                                     NULL,
                                     G_DBUS_CALL_FLAGS_NONE,
                                     -1,
                                     NULL,
                                     &error);
    if (error) {
        if (e && *e)
            *e = new MmError(error->message);
        g_warning ("%s\n", error->message);
    }

    g_variant_unref(ret);
}

void LMSProvider::stopIndexing (MmError **e) {
    GError *error = NULL;
    GVariant *ret;

    ret = g_dbus_connection_call_sync (connection,
                                     "org.lightmediascanner",
                                     "/org/lightmediascanner/Scanner1",
                                     "org.lightmediascanner.Scanner1",
                                     "Stop",
                                     NULL,
                                     NULL,
                                     G_DBUS_CALL_FLAGS_NONE,
                                     -1,
                                     NULL,
                                     &error);
    if (error) {
        if (e && *e)
            *e = new MmError(error->message);
        g_warning ("D-Bus error: %s\n", error->message);
    }

    if (ret)
        g_variant_unref(ret);
}

static void onNameAppeared (GDBusConnection *connection,
                     const gchar     *name,
                     const gchar     *name_owner,
                     void            *user_data)
{
    g_print("Found lightmediascannerd on D-Bus\n");
    ((LMSProvider *) user_data)->connection = connection;
    (*onConnectedCallback) (NULL);
}

static void onNameVanished (GDBusConnection *connection,
                     const gchar     *name,
                     void        *user_data)
{
    g_printerr ("Failed to get name owner for %s\n",
                name);
    (*onConnectedCallback) (new MmError("Failed to connect to LMS, is lightmediscannerd running?"));
}

bool LMSProvider::connect(std::function<void(MmError *e)> cb) {
    onConnectedCallback = new std::function<void(MmError *e)> (cb);

    uint m_watcherId = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                   "org.lightmediascanner",
                                   G_BUS_NAME_WATCHER_FLAGS_NONE,
                                   onNameAppeared,
                                   onNameVanished,
                                   this,
                                   NULL);
}

bool LMSProvider::isConnected() {
    return (connection && 
            !g_dbus_connection_is_closed (connection));
}

void LMSProvider::disconnect() {
    g_bus_unwatch_name (m_watcherId);
}

LMSProvider::~LMSProvider() {
    disconnect();
}
