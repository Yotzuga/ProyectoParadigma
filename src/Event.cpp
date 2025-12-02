#include "Event.h"

// CollisionEvent implementation
CollisionEvent::CollisionEvent(CollisionType type, uint32_t idA, uint32_t idB, float pushX, float pushY)
    : collisionType(type), entityA_Id(idA), entityB_Id(idB), pushDirectionX(pushX), pushDirectionY(pushY)
{
}

std::string CollisionEvent::getType() const
{
    return "CollisionEvent";
}

// DamageEvent implementation
DamageEvent::DamageEvent(uint32_t id, int dmg)
    : entityId(id), damage(dmg)
{
}

std::string DamageEvent::getType() const
{
    return "DamageEvent";
}

// SpawnEvent implementation
SpawnEvent::SpawnEvent(const std::string &type, float posX, float posY)
    : entityType(type), x(posX), y(posY)
{
}

std::string SpawnEvent::getType() const
{
    return "SpawnEvent";
}

// ========================================
// WAVE SYSTEM EVENTS
// ========================================

// SpawnEnemyEvent implementation
SpawnEnemyEvent::SpawnEnemyEvent(const std::string &typeName, float speedMult)
    : enemyTypeName(typeName), speedMultiplier(speedMult)
{
}

std::string SpawnEnemyEvent::getType() const
{
    return "SpawnEnemyEvent";
}

// WaveStartEvent implementation
WaveStartEvent::WaveStartEvent(int wave, int phase)
    : waveNumber(wave), phaseId(phase)
{
}

std::string WaveStartEvent::getType() const
{
    return "WaveStartEvent";
}

// WaveEndEvent implementation
WaveEndEvent::WaveEndEvent(int wave)
    : waveNumber(wave)
{
}

std::string WaveEndEvent::getType() const
{
    return "WaveEndEvent";
}

// ObstacleSpawnEvent implementation
ObstacleSpawnEvent::ObstacleSpawnEvent(float posX, float posY)
    : x(posX), y(posY)
{
}

std::string ObstacleSpawnEvent::getType() const
{
    return "ObstacleSpawnEvent";
}