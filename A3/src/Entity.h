#pragma once

#include <string>
#include <tuple>

#include "Components.h"

class EntityManager;

using ComponentTuple =  std::tuple
                        <
                            CTransform,
                            CLifespan,
                            CInput,
                            CBoundingBox,
                            CAnimation,
                            CGravity,
                            CState
                        >;

class Entity
{
    friend class EntityManager;     // Allows EntityManager objects to access private Entity constructor

private:
    bool m_alive = true;
    size_t m_id = 0;
    std::string m_tag = "default";
    ComponentTuple m_components;

private:
    Entity(const size_t id, const std::string& tag);

public:
    bool isAlive() const;
    void destroy();
    const std::string& getTag() const;
    const size_t getId() const;

    template <typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has();
    }

    template <typename T, typename... TArgs>        // TArgs is 1 or more arguments, in this case the arguments passed to the constructor
    T& addComponent(TArgs&&... args)       // rvalue references
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(args)...);       // Overwrites the current component
        component.has = true;
        return component;
    }
    
    template <typename T>
    const T& getComponent() const
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};