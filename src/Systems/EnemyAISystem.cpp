#include "../../include/Systems/EnemyAISystem.h"
#include "../../include/Entity.h"
#include "../../include/Components/EnemyComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformComponent.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

EnemyAISystem::EnemyAISystem()
{
}

void EnemyAISystem::update(World &world, float dt)
{
    // ✅ Optimización: Acceso directo al jugador
    Entity *player = world.GetPlayer();
    if (!player)
        return;

    auto *playerTransform = player->GetComponent<TransformComponent>();
    if (!playerTransform)
        return;

    // ✅ Actualizar solo enemigos
    const auto &enemies = world.GetEnemies();
    for (Entity *enemy : enemies)
    {
        auto *enemyComp = enemy->GetComponent<EnemyComponent>();
        auto *enemyTransform = enemy->GetComponent<TransformComponent>();

        if (!enemyComp || !enemyTransform)
            continue;

        // Calcular distancia al jugador
        glm::vec2 direction = playerTransform->m_Position - enemyTransform->m_Position;
        float distance = glm::length(direction);

        // Solo modificar velocidad si el jugador está dentro del rango
        if (distance <= enemyComp->focusRange && distance > 0.1f)
        {
            direction = glm::normalize(direction);
            glm::vec2 targetVelocity = direction * enemyComp->baseSpeed;

            float lerpFactor = enemyComp->velocitySmoothing * dt;
            enemyTransform->m_Velocity = glm::mix(enemyTransform->m_Velocity, targetVelocity, lerpFactor);
        }
    }
}
