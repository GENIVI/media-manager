#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "jansson.h"

#include "browserprovider.h"
#include "common.h"

#define ROOT_CONTAINER "/com/intel/dLeynaServer/server/1" 

class BrowserProviderTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
        browserprovider.disconnect();
    }

    BrowserProvider browserprovider;
    GMainLoop *loop;
};

TEST_F(BrowserProviderTest, Connect) {
    browserprovider.connect([&](MmError *browserError) {
        g_main_loop_quit (loop);
        ASSERT_TRUE (browserError == NULL);
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}

TEST_F(BrowserProviderTest, ListContainersRoot) {
    browserprovider.connect([&](MmError *browserError) {
        std::vector<std::string> filter;
        std::string containers;
        MmError *listContainersError;
        json_t *containersJson, *value;
        json_error_t jsonError;
        size_t index;

        filter.push_back("DisplayName");
        filter.push_back("Path");

        browserprovider.listContainers(ROOT_CONTAINER,
                                0,
                                100,
                                filter,
                                containers,
                                &listContainersError);

        g_main_loop_quit (loop);

        containersJson = json_loads (containers.c_str(), 0, &jsonError);

        json_array_foreach(containersJson, index, value) {
            ASSERT_TRUE (json_object_get (value, "DisplayName") != NULL);
            ASSERT_TRUE (json_object_get (value, "Path") != NULL);
        }
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
