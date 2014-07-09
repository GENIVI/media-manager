#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "lms.h"
#include "common.h"

class LMSProviderTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
        lmsprovider.disconnect();
    }

    LMSProvider lmsprovider;
    GMainLoop *loop;
};

TEST_F(LMSProviderTest, GetDatabasePath) {
    lmsprovider.connect([&](MmError *lmsError) {
        ASSERT_TRUE (lmsError == NULL);

        MmError *pathError = NULL;
        std::string dbp;
        lmsprovider.getDatabasePath(dbp, &pathError);

        ASSERT_TRUE (pathError == NULL);
        ASSERT_TRUE (dbp.length() > 0);
        std::cout << "Database path:" << dbp << std::endl;
        g_main_loop_quit(loop);
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

}

TEST_F(LMSProviderTest, StartIndexing) {
    lmsprovider.connect([&](MmError *lmsError) {
        ASSERT_TRUE (lmsError == NULL);

        MmError *startError = NULL;
        lmsprovider.startIndexing(&startError);

        ASSERT_TRUE (startError == NULL);
    });

    g_main_loop_new (NULL, TRUE);
}

TEST_F(LMSProviderTest, StopIndexing) {
    lmsprovider.connect([&](MmError *lmsError) {
        ASSERT_TRUE (lmsError == NULL);

        MmError *stopError = NULL;
        lmsprovider.stopIndexing(&stopError);

        ASSERT_TRUE (stopError == NULL);
    });

    g_main_loop_new (NULL, TRUE);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
