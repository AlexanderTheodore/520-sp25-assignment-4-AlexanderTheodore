#include <TestHarness.h>

#include "Hunter.h"

#include "Arrow.h"
#include "Wumpus.h"

#include "TestHelperTestEnvironment.h"
#include "UserNotification.h"
#include <vector>

namespace TestHuntTheWumpus
{
    TEST(HunterSuite, Hunter_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Hunter hunter(env.m_context);

        const auto &properties = hunter.Properties();

        CHECK(properties.m_carryableByBats);
        CHECK(properties.m_isEdible);
        CHECK(properties.m_reportMovement);
        CHECK(!properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(1, hunter.GetPriority());

        const auto& id = hunter.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Hunter, id.m_category);
    }

    TEST(HunterSuite, Hunter_RetrieveArrow_ReturnsValidArrow)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        const auto arrow = hunter.GetArrow();

        CHECK_EQUAL(HuntTheWumpus::Category::Arrow, arrow->GetIdentifier().m_category);

        CHECK(!hunter.OutOfArrows());
    }

    TEST(HunterSuite, Hunter_RetrieveAllArrows_ReportsEmpty)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        for (auto idx = 0; idx < 5; ++idx)
        {
            const auto arrow = hunter.GetArrow();
        }

        CHECK(hunter.OutOfArrows());
    }

    TEST(HunterSuite, Hunter_OnCaveEntrance_EatenByWumpus)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        bool callbackTriggered;
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::HunterEaten, [&callbackTriggered]() {callbackTriggered = true; });

        const auto wumpus = std::make_shared<HuntTheWumpus::Wumpus>(0, env.m_context);

        // This should return true that there was an action taken.
        CHECK(hunter.ObserveCaveEntrance(wumpus));

        // Show that a state-change happened to a "lost" result.
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult);
        CHECK(callbackTriggered);
    }

    TEST(HunterSuite, Hunter_OnCaveEntrance_ShotByArrow)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        const auto arrow = std::make_shared<HuntTheWumpus::Arrow>(0, env.m_context);

        bool callbackTriggered;
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::HunterShot, [&callbackTriggered]() {callbackTriggered = true; });


        // This should return true that there was an action taken.
        CHECK(hunter.ObserveCaveEntrance(arrow));

        // Show that a state-change happened to a "lost" result.
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult)
        CHECK(callbackTriggered);
    }

    // show that report neighboring caves is triggered and provides the correct neighboring caves as arguments in its callback
    TEST(HunterSuite, Hunter_ObservesCaveEntranceCallback)
    {
        TestEnvironment env;

        auto cave = std::make_shared<HuntTheWumpus::Cave>(57, env.m_dungeon);

        const auto cave1 = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);
        const auto cave2 = std::make_shared<HuntTheWumpus::Cave>(59, env.m_dungeon);
        const auto cave3 = std::make_shared<HuntTheWumpus::Cave>(60, env.m_dungeon);

        cave->ConnectTo(cave1);
        cave->ConnectTo(cave2);
        cave->ConnectTo(cave3);

        int enteredId;
        std::vector<int> neighboringIds;

        std::shared_ptr<HuntTheWumpus::Hunter> hunter = std::make_shared<HuntTheWumpus::Hunter>(env.m_context);

        env.m_userNotifier.AddCallback<std::pair<int, std::vector<int>>>(HuntTheWumpus::UserNotification::Notification::ReportNeighboringCaves, [&enteredId, &neighboringIds](const std::pair<int, std::vector<int>>& args) {
            enteredId = args.first;
            neighboringIds = args.second;
            }
        );

        hunter->EnterCave(cave);
        cave->AddDenizen(hunter, true);

        CHECK_EQUAL(enteredId, 57);
        CHECK_EQUAL(neighboringIds.at(0), 58);
        CHECK_EQUAL(neighboringIds.at(1), 59);
        CHECK_EQUAL(neighboringIds.at(2), 60);
    }
}
