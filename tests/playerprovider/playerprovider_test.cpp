#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "jansson.h"

#include "playerprovider.h"
#include "common.h"

class PlayerProviderTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
        playerprovider.disconnect();
    }

    PlayerProvider playerprovider;
    GMainLoop *loop;
};

TEST_F(PlayerProviderTest, Connect) {
    playerprovider.connect([&](MmError *playerError) {
        g_main_loop_quit (loop);
        ASSERT_TRUE (playerError == NULL);
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}

TEST_F(PlayerProviderTest, OpenURITest) {
    playerprovider.connect([&](MmError *playerError) {
        MmError *openURIError = NULL;
        std::string uri = "file:///home/jonatan/Music/Keinzweiter_-_Mircoobee.mp3";

        playerprovider.openURI(uri,
                               &openURIError);

        g_main_loop_quit (loop);

    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
