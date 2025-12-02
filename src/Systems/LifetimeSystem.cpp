#include "../../include/Systems/LifetimeSystem.h"
#include "../../include/World.h"
#include "../../include/Entity.h"
#include "../../include/Event.h"
#include <spdlog/spdlog.h>
#include <queue>

void LifetimeSystem::update(World &world, float dt)
{
    // ✅ PROPÓSITO ÚNICO: Escuchar WaveEndEvent y eliminar todos los enemigos Y obstáculos de esa ola
    // Procesar solo los eventos actuales, no los que re-emitamos
    std::queue<std::unique_ptr<Event>> eventsToReemit;
    std::unique_ptr<Event> event;

    while (world.poll(event))
    {
        if (event->getType() == "WaveEndEvent")
        {
            auto *waveEndEvent = static_cast<WaveEndEvent *>(event.get());
            spdlog::info("LifetimeSystem: Recibido WaveEndEvent (Ola {}) - Eliminando enemigos y obstáculos", waveEndEvent->waveNumber);

            // ✅ PASO 1: Eliminar todos los enemigos usando caché
            int enemiesKilled = 0;
            std::vector<uint32_t> entitiesToKill;

            const auto &enemies = world.GetEnemies(); // ✅ Usar caché, no GetAllEntities()
            for (const auto &entity : enemies)
            {
                entitiesToKill.push_back(entity->m_Id); // agregar todos los ids a la lista a eliminar
                enemiesKilled++;
            }

            for (uint32_t id : entitiesToKill) // eliminar enemigos por id
            {
                world.deleteEntity(id);
            }

            if (enemiesKilled > 0)
            {
                spdlog::info("LifetimeSystem: {} enemigos eliminados al finalizar ola {}", enemiesKilled, waveEndEvent->waveNumber);
            }

            // ✅ PASO 2: Eliminar todos los obstáculos usando caché
            int obstaclesKilled = 0;
            entitiesToKill.clear();

            const auto &obstacles = world.GetObstacles(); // ✅ Usar caché, no GetAllEntities()
            for (const auto &entity : obstacles)
            {
                entitiesToKill.push_back(entity->m_Id);
                obstaclesKilled++;
            }

            for (uint32_t id : entitiesToKill)
            {
                world.deleteEntity(id);
            }

            if (obstaclesKilled > 0)
            {
                spdlog::info("LifetimeSystem: {} obstáculos eliminados al finalizar ola {}", obstaclesKilled, waveEndEvent->waveNumber);
            }

            // Evento procesado, no re-emitir
        }
        else
        {
            // ⚠️ No es mi evento, guardarlo para re-emitir después
            eventsToReemit.push(std::move(event));
        }
    }

    // Re-emitir todos los eventos que no procesamos
    while (!eventsToReemit.empty())
    {
        world.emit(std::move(eventsToReemit.front()));
        eventsToReemit.pop();
    }
}
