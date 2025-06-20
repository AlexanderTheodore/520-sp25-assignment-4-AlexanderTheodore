#pragma once

#include <functional>
#include <vector>
#include <map>
#include <typeinfo>
#include <variant>

// can accept any function variant and allows for casting to specific argument types or no arguments at all in a single container
template<typename VariantType>
class EventHandlerVariant
{
public:
    // notifies all contained functions with no arguments
    void Notify();
    // notifies all contained functions with a templated argument
    template<typename Args>
    void Notify(const Args& argument);
    // adds a callback function that matches one of the types within variant type
    void AddCallback(VariantType&& newFunc);

private:
    // helper function that casts a variant container to a specific function without arugment and calls it
    void CastFunction(const VariantType& func);
    // helper function that casts a variant container to a specific function with an arugment and calls it
    template<typename functionType>
    void CastFunction(const VariantType& func, const functionType& arg);
    // contains all variant function containers
    std::vector<VariantType> m_list;
};

// defines all valid function variants this event handler will accept
class Eventhandler : public EventHandlerVariant< std::variant < std::function<void()>, std::function<void(int)>, std::function<void(std::vector<int>)>>>
{
};

template<typename VariantType>
void EventHandlerVariant<VariantType>::Notify()
{
    for (const auto& listeners : m_list)
    {
        CastFunction(listeners);
    }
}

template<typename VariantType>
void EventHandlerVariant<VariantType>::AddCallback(VariantType&& newFunc)
{
    m_list.emplace_back(newFunc);
}

template<typename VariantType>
void EventHandlerVariant<VariantType>::CastFunction(const VariantType& func)
{
    auto* cast = std::get_if<std::function<void()>>(&func);
    if (cast)
    {
        (*cast)();
    }
}

template<typename VariantType>
template<typename Args>
void EventHandlerVariant<VariantType>::Notify(const Args& argument)
{
    for (const auto& listeners : m_list)
    {
        CastFunction<Args>(listeners, argument);
    }
}

template<typename VariantType>
template<typename functionType>
void EventHandlerVariant<VariantType>::CastFunction(const VariantType& func, const functionType& arg)
{
    auto* cast = std::get_if<std::function<void(functionType)>>(&func);
    if (cast)
    {
        (*cast)(arg);
    }
}
