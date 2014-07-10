PROJECT (BrowserProviderTests)

INCLUDE (../../common.cmake)

INCLUDE_DIRECTORIES (${GMOCK_INCLUDE_DIR}
                     ${GTEST_INCLUDE_DIR}
                     ${CMAKE_SOURCE_DIR}/src/)

SET (SOURCES
        ${CMAKE_SOURCE_DIR}/src/playerprovider.cpp
        ${CMAKE_SOURCE_DIR}/src/serviceprovider.cpp
        ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.c
        playerprovider_test.cpp
)

SET (HEADERS
        ${CMAKE_SOURCE_DIR}/src/playerprovider.h
        ${CMAKE_SOURCE_DIR}/src/serviceprovider.h
        ${MEDIAMANAGER_BINARY_DIR}/generated/dleyna/dleyna-renderer-generated.h
)
ADD_EXECUTABLE (PlayerProviderTests ${SOURCES} ${HEADERS})

TARGET_LINK_LIBRARIES (PlayerProviderTests
                       ${PC_GIO_LIBRARIES}
                       gtest
                       gmock)
