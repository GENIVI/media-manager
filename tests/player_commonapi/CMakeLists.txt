PROJECT (PlayerCommonAPITest)

INCLUDE (../../common.cmake)

INCLUDE_DIRECTORIES (${GMOCK_INCLUDE_DIR}
                     ${GTEST_INCLUDE_DIR}
                     ${CMAKE_SOURCE_DIR}/src/)

SET (SOURCES
        player_commonapi_test.cpp
)

SET (HEADERS
)
ADD_EXECUTABLE (PlayerCommonAPITest ${SOURCES}
                                    ${HEADERS}
                                    ${GENERATED_SOURCES_CAPI}
                                    ${GENERATED_HEADERS_CAPI})

TARGET_LINK_LIBRARIES (PlayerCommonAPITest
                       ${PC_GIO_LIBRARIES}
                       ${PC_COMMONAPI_LIBRARIES}
                       ${PC_COMMONAPIDBUS_LIBRARIES}
                       gtest
                       gmock)
