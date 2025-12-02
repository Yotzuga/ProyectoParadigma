#include "../../include/Systems/DamageSystem.h"
#include "../../include/Entity.h"
#include "../../include/Components/HealthComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Event.h"
#include <spdlog/spdlog.h>

DamageSystem::DamageSystem(bool *gameRunning, bool *gameOver, float invulnerabilityDuration)
    : m_GameRunning(gameRunning),
      m_GameOver(gameOver),
      m_PlayerInvulnerabilityDuration(invulnerabilityDuration)
{
}

void DamageSystem::update(World &world, float dt)
{
    // 1. Actualizar invulnerabilidad
    Entity *player = world.GetPlayer();
    if (player)
    {
        auto *playerComp = player->GetComponent<PlayerComponent>();
        if (playerComp)
        {
            playerComp->UpdateInvulnerability(dt);
        }
    }

    // 2. Procesar todos los eventos DamageEvent
    std::unique_ptr<Event> event;
    while (world.poll(event))
    {
        if (event->getType() == "DamageEvent")
        {
            DamageEvent *damageEvent = static_cast<DamageEvent *>(event.get());

            spdlog::warn("Procesando DamageEvent: Entidad={}, Danio={}",
                         damageEvent->entityId, damageEvent->damage);

            // Buscar la entidad objetivo (O(1) con el nuevo GetEntityById)
            Entity *targetEntity = world.FindEntityById(damageEvent->entityId);
            if (targetEntity)
            {
                // Verificar si es el jugador y está invulnerable
                auto *playerComp = targetEntity->GetComponent<PlayerComponent>();
                if (playerComp && playerComp->IsInvulnerable())
                {
                    spdlog::info("¡Jugador invulnerable! Daño bloqueado");
                    continue; // El daño no aplica
                }

                auto *health = targetEntity->GetComponent<HealthComponent>();
                if (health)
                {
                    // Reducir HP usando takeDamage
                    health->takeDamage(damageEvent->damage);

                    spdlog::info("Entidad {} recibio {} de danio. HP: {}/{}",
                                 targetEntity->m_Id, damageEvent->damage,
                                 health->hp, health->maxHp);

                    // Si es el jugador, activar invulnerabilidad
                    if (playerComp)
                    {
                        playerComp->SetInvulnerable(m_PlayerInvulnerabilityDuration); // Duración desde settings.json
                        spdlog::info("Jugador activo invulnerabilidad temporal ({:.1f} segundos)", m_PlayerInvulnerabilityDuration);
                    }

                    // Verificar si murio
                    if (health->isDead())
                    {
                        spdlog::error("GAME OVER Entidad {} murio", targetEntity->m_Id);

                        // Activar Game Over
                        if (m_GameOver)
                        {
                            *m_GameOver = true;
                        }
                    }
                }
            }
        }
    }
}
