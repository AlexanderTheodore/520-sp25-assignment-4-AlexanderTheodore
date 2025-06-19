#pragma once

#include <functional>
#include <map>
#include "EventHandler.h"

namespace HuntTheWumpus
{
    // Inform the user about particular operations happening.
    class UserNotification final
    {
    public:

        enum class Notification
        {
            ObserveWumpus,
            ObservePit,
            ObserveBat,
            ObserveMiss,
            ObserveOutOfArrows,
            BatTriggered,
            PitTriggered,
            WumpusTriggered,
            WumpusAwoken,
            WumpusShot,
            HunterEaten,
            HunterShot,
            CaveEntered, // requires vector<denizens> argument
            ReportIllegalMove, // requires cave id arguments
            ReportNeighboringCaves // requires vector<int> arguments
        };

        UserNotification() = default;
        ~UserNotification() = default;

        void AddCallback(Notification category, std::function<void()>&& callback);

        template<typename Arg>
        void AddCallback(Notification category, std::function<void(Arg)>&& callback);


        // TODO: implement
        void Notify(Notification category);

        template<typename Arg>
        void Notify(Notification category, Arg args);

        UserNotification(const UserNotification&) = default;
        UserNotification(UserNotification&&) = default;
        UserNotification& operator=(const UserNotification&) = default;
        UserNotification& operator=(UserNotification&&) = default;

        // TODO: hold callbacks.
    private:
        std::map<Notification, EventHandler> m_callbacks;
        EventHandler& GetEventHandler(Notification category);
    };

    template<typename Arg>
    void UserNotification::AddCallback(Notification category, std::function<void(Arg)>&& callback)
    {
        GetEventHandler(category).AddCallback<Arg>(std::move(callback));
    }
    template<typename Arg>
    void UserNotification::Notify(Notification category, Arg args)
    {
        GetEventHandler(category).Notify<Arg>(args);
    }

}