#pragma once

#include "Components.h"

#include <string>
#include <tuple>

class EntityManager;

using ComponentTuple =  std::tuple
                        <
                            CTransform,
                            CBody,
                            CLifespan,
                            CInput,
                            CAnimation,
                            CGravity,
                            CState
                        >;

class Entity
{
    friend class EntityManager;     // Allows EntityManager objects to access private Entity constructor

public:
    template <typename T, typename... TArgs>                // TArgs is 1 or more arguments, in this case the arguments passed to the constructor
    T& addComponent(TArgs&&... args)                        // rvalue references
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(args)...);        // Overwrites the current component
        component.has = true;
        return component;
    }
    
    template <typename T>
    void removeComponent()
    {
		getComponent<T>() = T();                            // component.has set to false
    }

    template <typename T>
    T& getComponent()
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    const T& getComponent() const
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    bool hasComponent()
    {
        return getComponent<T>().has;
    }

    void destroy();
    bool isAlive() const;
    const size_t getId() const;
    const std::string& getTag() const;

private:
    ComponentTuple m_components;
    bool m_alive = true;
    size_t m_id = 0;
    std::string m_tag = "default";

    Entity(const size_t id, const std::string& tag);
};