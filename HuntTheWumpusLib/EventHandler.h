#include <functional>
#include <vector>
#include <map>
#include <typeinfo>
#include <memory>
#include <typeindex>



struct IFunctionContainer
{

};

template<typename Args>
struct TemplateFunctionContainter : IFunctionContainer
{
	std::vector<std::function<void(Args)>> m_callbacks;
};

class EventHandler 
{
public:
	template<typename Args>
	void AddCallback(std::function<void(Args)>&&);
	template<typename Args>
	void Notify(Args);
	void AddCallback(std::function<void()>&&);
	void Notify();
private:
	std::vector<std::function<void()>> m_callbacks;
	std::map<std::type_index, IFunctionContainer*> m_templateCallbacks;

	template<typename Args>
	std::vector<std::function<void(Args)>>& GetTemplatedCallbacks();
};

template<typename Args>
void EventHandler::AddCallback(std::function<void(Args)>&& callback)
{
	GetTemplatedCallbacks<Args>().emplace_back(callback);
}

template<typename Args>
void EventHandler::Notify(Args arguments)
{
	for (auto callback : GetTemplatedCallbacks<Args>())
	{
		callback(arguments);
	}
}

void EventHandler::AddCallback(std::function<void()>&& callback)
{
	m_callbacks.emplace_back(callback);
}

void EventHandler::Notify()
{
	for (auto callback : m_callbacks)
	{
		callback();
	}
}

// this feels really wrong but I'm unsure how to account for a single event handler object that can handle different arguments 
template<typename Args>
std::vector<std::function<void(Args)>>& EventHandler::GetTemplatedCallbacks()
{
	std::type_index key = typeid(Args);
	if (!m_templateCallbacks.contains(key))
	{
		m_templateCallbacks.try_emplace(key, new TemplateFunctionContainter<Args>);
	}
	TemplateFunctionContainter<Args>* cast = static_cast<TemplateFunctionContainter<Args>*>(m_templateCallbacks.at(key));
	return cast->m_callbacks;
}
