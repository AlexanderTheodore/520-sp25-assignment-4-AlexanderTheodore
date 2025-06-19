#include <functional>
#include <vector>

class IEventHandler
{
};

template<typename Args>
class EventHandler : public IEventHandler
{
public:
	void AddCallback(std::function<void(Args)>&&);
	void Notify(Args);
private:
	std::vector<std::function<void(Args)>> m_callbacks;
};

template<typename Args>
void EventHandler<Args>::AddCallback(std::function<void(Args)>&& callback)
{
	m_callbacks.emplace_back(callback);
}

template<typename Args>
void EventHandler<Args>::Notify(Args arguments)
{
	for (auto callback : m_callbacks)
	{
		callback(arguments);
	}
}
template<>
void EventHandler<void>::Notify()
{
	for (auto callback : m_callbacks)
	{
		callback();
	}
}
