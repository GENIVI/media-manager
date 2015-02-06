PROJECT (BrowserProviderTests)

INCLUDE (../../common.cmake)

INCLUDE_DIRECTORIES (${GMOCK_INCLUDE_DIR}
                     ${GTEST_INCLUDE_DIR}
                     ${CMAKE_SOURCE_DIR}/src/)

SET (SOURCES
        ${CMAKE_SOURCE_DIR}/src/common.cpp
        ${CMAKE_SOURCE_DIR}/src/browserprovider.cpp
        ${CMAKE_SOURCE_DIR}/src/serviceprovider.cpp
        ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.c
        browserprovider_test.cpp
)

SET (HEADERS
        ${CMAKE_SOURCE_DIR}/src/common.h
        ${CMAKE_SOURCE_DIR}/src/browserprovider.h
        ${CMAKE_SOURCE_DIR}/src/serviceprovider.h
        ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-generated.h
)
ADD_EXECUTABLE (BrowserProviderTests ${SOURCES} ${HEADERS})

TARGET_LINK_LIBRARIES (BrowserProviderTests
                       ${PC_GIO_LIBRARIES}
                       ${PC_JANSSON_LIBRARIES}
                       ${PC_COMMONAPI_LIBRARIES}
                       ${PC_COMMONAPIDBUS_LIBRARIES}
                       gtest
                       gmock)
