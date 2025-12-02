#include "../../include/Components/HealthComponent.h"
#include <algorithm>

// std::max usado en takeDamage

HealthComponent::HealthComponent(int maxHealth)
    : Component(), hp(maxHealth), maxHp(maxHealth)
{
    m_Type = "HealthComponent";
}

void HealthComponent::takeDamage(int damage)
{
    hp = std::max(0, hp - damage);
}

bool HealthComponent::isDead() const
{
    return hp <= 0;
}
