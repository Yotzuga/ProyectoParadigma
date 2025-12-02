#include "Component.h"

Component::Component() : m_Type("Component")
{
}

const std::string &Component::getType() const
{
    return m_Type;
}

Component::~Component() = default;