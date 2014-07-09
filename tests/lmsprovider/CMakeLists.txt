PROJECT (LMSProviderTests)

LIST (APPEND CMAKE_CXX_FLAGS "-std=c++11")

INCLUDE_DIRECTORIES (${GMOCK_INCLUDE_DIR}
                     ${GTEST_INCLUDE_DIR}
                     ${PC_GIO_INCLUDE_DIRS}
                     ${CMAKE_SOURCE_DIR}/src/)

MESSAGE(FATAL ${GMOCK_INCLUDE_DIR})

SET (SOURCES
        ${CMAKE_SOURCE_DIR}/src/lms.cpp
        lmsprovider_test.cpp
)

SET (HEADERS
        ${CMAKE_SOURCE_DIR}/src/lms.h
)
ADD_EXECUTABLE (LMSProviderTests ${SOURCES})

TARGET_LINK_LIBRARIES (LMSProviderTests
                       ${PC_GIO_LIBRARIES}
                       gtest
                       gmock)
