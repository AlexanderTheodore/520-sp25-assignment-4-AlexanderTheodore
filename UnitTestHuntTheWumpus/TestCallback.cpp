#include <TestHarness.h>

#include "UserNotification.h"
#include "EventHandler.h"

#include "TestHelperTestEnvironment.h"

#include <sstream>
#include <variant>
#include <cassert>

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

    // show that the event handler can contain any number of callbacks but only triggers the appropriate templated callback
    TEST(CallbackSuite, EventHandlerTestNoParams)
    {
        std::stringstream output;
        Eventhandler intCallback;
        // add parameterless, int and string callbacks
        intCallback.AddCallback([&]() {output << "I dont care about your number!"; });
        intCallback.AddCallback([&](int value) {output << "Your number is " << value; });
        // ONLY notify int callback
        intCallback.Notify();

        CHECK_EQUAL(output.str(), "I dont care about your number!");
    }

    // show that the event handler can contain any number of callbacks but only triggers the appropriate templated callback
    TEST(CallbackSuite, EventHandlerTestTemplate)
    {
        std::stringstream output;
        Eventhandler intCallback;
        // add parameterless, int and string callbacks
        intCallback.AddCallback([&]() {output << "I dont care about your number!"; });
        intCallback.AddCallback([&](int value) {output << "Your number is " << value; });
        // ONLY notify int callback
        intCallback.Notify(8);

        CHECK_EQUAL(output.str(), "Your number is 8");
    }
}
