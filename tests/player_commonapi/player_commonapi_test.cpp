#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "jansson.h"

#include <org/genivi/mediamanager/PlayerProxy.h>
#include "common.h"

class PlayerCommonAPITest : public ::testing::Test {
protected:
    bool initialize() {
        CommonAPI::Runtime::LoadState loadState;
        auto runtime = CommonAPI::Runtime::load(loadState);
        if (loadState != CommonAPI::Runtime::LoadState::SUCCESS) {
            std::cerr << "Error: Unable to load runtime!\n";
            return false;
        }

        auto factory = runtime->createFactory();
        if (!factory) {
            std::cerr << "Error: Unable to create factory!\n";
            return false;
        }
        const std::string commonApiAddress = "local:org.genivi.mediamanager.Player:org.genivi.mediamanager.Player";
        playerProxy = factory->buildProxy<org::genivi::mediamanager::PlayerProxy>(commonApiAddress);
        if (!playerProxy) {
            std::cerr << "Error: Unable to build player proxy!\n";
            return false;
        }

        std::promise<CommonAPI::AvailabilityStatus> availabilityStatusPromise;
        playerProxy->getProxyStatusEvent().subscribe([&](const CommonAPI::AvailabilityStatus& availabilityStatus) {
                availabilityStatusPromise.set_value(availabilityStatus);
                return CommonAPI::SubscriptionStatus::CANCEL;
        });

        auto availabilityStatusFuture = availabilityStatusPromise.get_future();
        availabilityStatusFuture.wait();

        if (availabilityStatusFuture.get() != CommonAPI::AvailabilityStatus::AVAILABLE) {
            std::cerr << "Proxy not available! Did you start media-manager"
                      << " before running the test?" << std::endl;
            return false;
        }

        return true;
    }

    virtual void SetUp() {
        ASSERT_TRUE (initialize());
    }

    virtual void TearDown() {
    }
    std::shared_ptr<org::genivi::mediamanager::PlayerProxy<> > playerProxy;
};

TEST_F(PlayerCommonAPITest, Connect) {
    // The fixture will ensure connecting is successful, and fail otherwise
}

TEST_F(PlayerCommonAPITest, OpenURITest) {
    CommonAPI::CallStatus callStatus;
    org::genivi::mediamanager::PlayerTypes::PlayerError error;

    playerProxy->openUri ("file:///some-file", callStatus, error);
    ASSERT_TRUE (error == org::genivi::mediamanager::PlayerTypes::PlayerError::NO_ERROR);
}

TEST_F(PlayerCommonAPITest, PlayTest) {
    CommonAPI::CallStatus callStatus;
    org::genivi::mediamanager::PlayerTypes::PlayerError error;

    playerProxy->play(callStatus, error);
    ASSERT_TRUE (error == org::genivi::mediamanager::PlayerTypes::PlayerError::NO_ERROR);
}

TEST_F(PlayerCommonAPITest, PauseTest) {
    CommonAPI::CallStatus callStatus;
    org::genivi::mediamanager::PlayerTypes::PlayerError error;

    playerProxy->pause(callStatus, error);
    ASSERT_TRUE (error == org::genivi::mediamanager::PlayerTypes::PlayerError::NO_ERROR);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
