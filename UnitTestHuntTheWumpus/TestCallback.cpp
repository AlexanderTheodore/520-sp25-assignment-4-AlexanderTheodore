#include <TestHarness.h>

#include "UserNotification.h"
#include "EventHandler.h"

#include "TestHelperTestEnvironment.h"

#include <sstream>

namespace TestHuntTheWumpus
{
    TEST(CallbackSuite, RegisterTest)
    {
        HuntTheWumpus::UserNotification observer;
        std::stringstream output;
        observer.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [&]() {output << "Main Callback1"; });
        observer.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [&]() {output << "Main Callback2"; });
        observer.AddCallback(HuntTheWumpus::UserNotification::Notification::ReportIllegalMove, [&]() {output << "Non main callback, shouldn't be called"; });
        
        observer.Notify(HuntTheWumpus::UserNotification::Notification::CaveEntered);

        CHECK_EQUAL(output.str(), "Main Callback1Main Callback2")
    }

    TEST(CallbackSuite, EventHandlerTest)
    {
        std::stringstream output;
        EventHandler<int> intCallback;
        EventHandler<void> regCallback;
        regCallback.AddCallback([&]() {output << "I dont care about your number!"; });
        regCallback.Notify();
        intCallback.AddCallback([&](int value) {output << "Your number is " << value; });
        intCallback.Notify(8);

        CHECK_EQUAL(output.str(), "I dont care about your number!Your number is 8");
;    }
}
