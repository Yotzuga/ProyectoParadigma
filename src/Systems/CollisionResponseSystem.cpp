#include "../../include/Systems/CollisionResponseSystem.h"
#include "../../include/Game.h"
#include "../../include/Entity.h"
#include "../../include/Components/TransformComponent.h"
#include "../../include/Components/ColliderComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/EnemyComponent.h"
#include "../../include/Event.h"
#include <spdlog/spdlog.h>
#include <vector>

CollisionResponseSystem::CollisionResponseSystem(const GameContext &ctx, float playerPush, float enemyPush)
    : m_WorldWidth(ctx.worldWidth),
      m_WorldHeight(ctx.worldHeight),
      m_PlayerPushStrength(playerPush),
      m_EnemyPushStrength(enemyPush)
{
}

void CollisionResponseSystem::update(World &world, float dt)
{
    std::unique_ptr<Event> event;
    std::vector<std::unique_ptr<Event>> otherEvents;

    while (world.poll(event))
    {
        if (event->getType() == "CollisionEvent")
        {
            CollisionEvent *collisionEvent = static_cast<CollisionEvent *>(event.get());

            // Obtener entidad A
            Entity *entityA = world.FindEntityById(collisionEvent->entityA_Id);
            if (!entityA)
                continue;

            auto *transformA = entityA->GetComponent<TransformComponent>();
            if (!transformA)
                continue;

            switch (collisionEvent->collisionType)
            {
            case CollisionType::PLAYER_ENEMY:
            {
                // Verificar invulnerabilidad del jugador
                auto *playerComp = entityA->GetComponent<PlayerComponent>();
                if (playerComp && playerComp->IsInvulnerable())
                {
                    // Jugador invulnerable, ignorar colisión
                    continue; // No aplicar daño ni push
                }

                // Aplicar push al jugador
                glm::vec2 pushDirection(collisionEvent->pushDirectionX, collisionEvent->pushDirectionY);
                transformA->m_Position += pushDirection * m_PlayerPushStrength;

                // Emitir DamageEvent al jugador
                auto damageEvent = std::make_unique<DamageEvent>(collisionEvent->entityA_Id, 1);
                world.emit(std::move(damageEvent));

                break;
            }

            case CollisionType::ENEMY_ENEMY:
            {
                // Obtener entidad B
                Entity *entityB = world.FindEntityById(collisionEvent->entityB_Id);
                if (!entityB)
                    continue;

                auto *transformB = entityB->GetComponent<TransformComponent>();
                if (!transformB)
                    continue;

                // Empujar ambos enemigos en direcciones opuestas
                glm::vec2 pushDirection(collisionEvent->pushDirectionX, collisionEvent->pushDirectionY);
                transformA->m_Position += pushDirection * m_EnemyPushStrength;
                transformB->m_Position -= pushDirection * m_EnemyPushStrength;

                // Invertir velocidades (rebote)
                transformA->m_Velocity *= -1.0f;
                transformB->m_Velocity *= -1.0f;

                break;
            }

            case CollisionType::ENTITY_WORLD:
            {
                // Obtener collider para clamping
                auto *collider = entityA->GetComponent<ColliderComponent>();
                if (!collider)
                    continue;

                glm::vec2 pushDirection(collisionEvent->pushDirectionX, collisionEvent->pushDirectionY);

                // Aplicar clamping y rebote
                if (pushDirection.x > 0.0f) // Chocó con borde izquierdo
                {
                    transformA->m_Position.x = 0.f;
                    transformA->m_Velocity.x *= -1.f;
                }
                else if (pushDirection.x < 0.0f) // Chocó con borde derecho
                {
                    transformA->m_Position.x = m_WorldWidth - collider->m_Bounds.x;
                    transformA->m_Velocity.x *= -1.f;
                }

                if (pushDirection.y > 0.0f) // Chocó con borde superior
                {
                    transformA->m_Position.y = 0.f;
                    transformA->m_Velocity.y *= -1.f;
                }
                else if (pushDirection.y < 0.0f) // Chocó con borde inferior
                {
                    transformA->m_Position.y = m_WorldHeight - collider->m_Bounds.y;
                    transformA->m_Velocity.y *= -1.f;
                }

                break;
            }

            case CollisionType::PLAYER_OBSTACLE:
            {
                auto *collider = entityA->GetComponent<ColliderComponent>();
                if (!collider)
                    continue;

                glm::vec2 pushDirection(collisionEvent->pushDirectionX, collisionEvent->pushDirectionY);
                transformA->m_Position += pushDirection * m_PlayerPushStrength;
                transformA->m_Velocity *= -1.0f;

                break;
            }

            case CollisionType::ENEMY_OBSTACLE:
            {
                auto *collider = entityA->GetComponent<ColliderComponent>();
                if (!collider)
                    continue;

                glm::vec2 pushDirection(collisionEvent->pushDirectionX, collisionEvent->pushDirectionY);
                transformA->m_Position += pushDirection * m_EnemyPushStrength;
                transformA->m_Velocity *= -1.0f;

                break;
            }
            }
        }
        else
        {
            // No es un CollisionEvent, guardarlo para re-emitirlo después
            otherEvents.push_back(std::move(event));
        }
    }

    // Re-emitir todos los eventos que no eran CollisionEvent
    for (auto &otherEvent : otherEvents)
    {
        world.emit(std::move(otherEvent));
    }
}
