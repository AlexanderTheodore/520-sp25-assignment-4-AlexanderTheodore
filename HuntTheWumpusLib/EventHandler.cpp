#include <functional>
#include <vector>
#include "EventHandler.h"

void EventHandler::AddCallback(std::function<void()>&& callback)
{
	m_callbacks.emplace_back(callback);
}

void EventHandler::Notify()
{
	for (const auto& callback : m_callbacks)
	{
		callback();
	}
}
