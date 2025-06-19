#include "UserNotification.h"
#include "EventHandler.h"

#include <functional>
#include <map>

namespace HuntTheWumpus
{
    void UserNotification::AddCallback([[maybe_unused]] const Notification category, [[maybe_unused]] std::function<void()>&& callback)
    {
        GetEventHandler(category).AddCallback(std::move(callback));
    }

    void UserNotification::Notify(Notification category)
    {
        GetEventHandler(category).Notify();
    }

    EventHandler& UserNotification::GetEventHandler(Notification category)
    {
        if (!m_callbacks.contains(category))
        {
            m_callbacks.emplace(category, EventHandler());
        }
        return m_callbacks.at(category);
    }
}
