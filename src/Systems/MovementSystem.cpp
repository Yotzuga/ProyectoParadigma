#include "../../include/Systems/MovementSystem.h"
#include "../../include/Entity.h"
#include "../../include/Components/TransformComponent.h"

MovementSystem::MovementSystem()
{
}

void MovementSystem::update(World &world, float dt)
{
    // Actualizar jugador
    Entity *player = world.GetPlayer();
    if (player)
    {
        auto *transform = player->GetComponent<TransformComponent>();
        if (transform)
        {
            transform->m_Position += transform->m_Velocity * dt;
        }
    }

    // Actualizar enemigos (caché optimizado)
    const auto &enemies = world.GetEnemies();
    for (const auto &enemy : enemies)
    {
        auto *transform = enemy->GetComponent<TransformComponent>();
        if (transform)
            transform->m_Position += transform->m_Velocity * dt;
    }
}
