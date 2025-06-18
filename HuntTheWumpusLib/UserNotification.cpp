#include "UserNotification.h"

#include <functional>
#include <map>

namespace HuntTheWumpus
{
    void UserNotification::AddCallback([[maybe_unused]] const Notification category, [[maybe_unused]] std::function<void()>&& callback)
    {
        m_callbacks.insert({ category, callback });
    }

    void UserNotification::Notify(Notification category) const
    {
        auto indexer = m_callbacks.equal_range(category);
        for (auto callback = indexer.first; callback != indexer.second; ++callback)
        {
            callback->second();
        }
    }

}
