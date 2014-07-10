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
