SET (CUSTOM_DBUS_PATH /usr/local/lib/)
SET (INTERFACE_DIR ${MEDIAMANAGER_SOURCE_DIR}/interfaces/src-gen/)

FIND_PACKAGE (PkgConfig)

pkg_check_modules (PC_GIO           REQUIRED gio-2.0)
pkg_check_modules (PC_COMMONAPI     REQUIRED CommonAPI)
pkg_check_modules (PC_COMMONAPIDBUS REQUIRED CommonAPI-DBus)
pkg_check_modules (PC_GIO_UNIX      REQUIRED gio-unix-2.0)
pkg_check_modules (PC_JANSSON       REQUIRED jansson)

INCLUDE_DIRECTORIES (${PC_GIO_INCLUDE_DIRS}
                     ${PC_COMMONAPI_INCLUDE_DIRS}
                     ${PC_COMMONAPIDBUS_INCLUDE_DIRS}
                     ${PC_GIO_UNIX_INCLUDE_DIRS}
                     ${PC_JANSSON_INCLUDE_DIRS})

INCLUDE_DIRECTORIES (${INTERFACE_DIR}
                     ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/)

LINK_DIRECTORIES (${CUSTOM_DBUS_PATH})

ADD_DEFINITIONS (${PC_COMMONAPIDBUS_CFLAGS} ${PC_GIO_UNIX_CFLAGS})

SET(CMAKE_CXX_FLAGS "-std=c++0x")

ADD_CUSTOM_COMMAND (OUTPUT ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.c
                           ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.h
                    COMMAND mkdir -p ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/ &&
                            gdbus-codegen --generate-c-code ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated
                                          --c-namespace dleynaServer
                                          --interface-prefix dleyna
                                          --interface-prefix org.gnome.UPnP
                                          ${MEDIAMANAGER_SOURCE_DIR}/src/interfaces/dleyna.xml)

ADD_CUSTOM_COMMAND (OUTPUT ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.c
                           ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.h
                    COMMAND mkdir -p ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/ &&
                            gdbus-codegen --generate-c-code ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated
                                          --c-namespace dleynaRenderer
                                          --interface-prefix dleyna
                                          --interface-prefix org.mpris
                                          ${MEDIAMANAGER_SOURCE_DIR}/src/interfaces/dleyna-renderer.xml)

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
