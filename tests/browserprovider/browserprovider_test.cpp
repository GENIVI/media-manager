#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "browserprovider.h"
#include "common.h"

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

        filter.push_back("DisplayName");

        browserprovider.listContainers("/com/intel/dLeynaServer/server/1",
                                0,
                                100,
                                filter,
                                containers,
                                &listContainersError);

        std::cout << containers << std::endl;
        g_main_loop_quit (loop);
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
