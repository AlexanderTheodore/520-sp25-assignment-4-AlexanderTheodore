#pragma once

#include <functional>
#include <map>

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

        // TODO: implement
        void Notify(Notification category) const;

        UserNotification(const UserNotification&) = default;
        UserNotification(UserNotification&&) = default;
        UserNotification& operator=(const UserNotification&) = default;
        UserNotification& operator=(UserNotification&&) = default;

        // TODO: hold callbacks.
    private:
        std::unordered_multimap<Notification, std::function<void()>> m_callbacks;

    };
}