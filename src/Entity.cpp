#include "Entity.h"
#include "Component.h"

Entity::Entity() : m_Id(0), m_Name("")
{
}

Entity::Entity(uint32_t id, const std::string &name) : m_Id(id), m_Name(name)
{
}

void Entity::AddComponent(std::unique_ptr<Component> component)
{
    m_Components.push_back(std::move(component));
}

uint32_t Entity::getId() const
{
    return m_Id;
}

std::string Entity::getName() const
{
    return m_Name;
}

Entity::~Entity()
{
    m_Components.clear();
}