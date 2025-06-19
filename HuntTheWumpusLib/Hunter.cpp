#include "Hunter.h"

#include "Arrow.h"

#include "Cave.h"
#include "Context.h"
#include "GameStateObservation.h"
#include "UserNotification.h"

namespace HuntTheWumpus
{
    Hunter::Hunter(Context& providers)
        : Denizen({ .m_category= Category::Hunter, .m_instance= 0}, {.m_carryableByBats= true, .m_fatalToWumpus= false,
                      .m_fatalToHunter= false, .m_isEdible= true, .m_reportMovement= true
                  }, providers)
    {
        for (auto arrowIdx = 0; arrowIdx < QuiverCapacity; ++arrowIdx)
        {
            m_arrows.emplace_back(std::make_shared<Arrow>(arrowIdx, providers));
        }
    }

    std::shared_ptr<Arrow> Hunter::GetArrow()
    {
        auto result = m_arrows.back();
        m_arrows.pop_back();

        return result;
    }

    bool Hunter::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        if (trigger->Properties().m_fatalToHunter)
        {
            if (trigger->GetIdentifier().m_category == Category::Arrow)
            {
                m_providers.m_notification.Notify(UserNotification::Notification::HunterShot);
            }
            else if (trigger->GetIdentifier().m_category == Category::Wumpus)
            {
                m_providers.m_notification.Notify(UserNotification::Notification::HunterEaten);
            }
            m_providers.m_change.GameOver(false);
            return true;
        }
        m_providers.m_notification.Notify<std::pair<int, std::vector<int>>>(HuntTheWumpus::UserNotification::Notification::ReportNeighboringCaves, {GetCurrentCave().lock()->GetCaveId(), GetCurrentCave().lock()->GetConnectedIds() });
        RaiseWarning();
        return false;
    }

    void Hunter::RaiseWarning()
    {
        for (auto caveId : GetCurrentCave().lock()->GetConnectedIds())
        {
            std::shared_ptr<Cave> neighboringCave = GetCurrentCave().lock()->GetConnectedCave(caveId).lock();
            if (neighboringCave->HasDenizen(DenizenIdentifier(Category::Wumpus)))
            {
                m_providers.m_notification.Notify(UserNotification::Notification::ObserveWumpus);
            }
            if (neighboringCave->HasDenizen(DenizenIdentifier(Category::Bat)))
            {
                m_providers.m_notification.Notify(UserNotification::Notification::ObserveBat);
            }
            if (neighboringCave->HasDenizen(DenizenIdentifier(Category::Pit)))
            {
                m_providers.m_notification.Notify(UserNotification::Notification::ObservePit);
            }
        }
    }

}
