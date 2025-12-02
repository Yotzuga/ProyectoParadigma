#include "../../include/Systems/SpawnSystem.h"
#include "../../include/EntityManager.h"
#include "../../include/Event.h"
#include <spdlog/spdlog.h>
#include <queue>

SpawnSystem::SpawnSystem(SDL_Renderer *renderer, float worldWidth, float worldHeight,
                         const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes)
    : m_Renderer(renderer),
      m_WorldWidth(worldWidth),
      m_WorldHeight(worldHeight),
      m_EnemyTypes(enemyTypes)
{
}

void SpawnSystem::update(World &world, float dt)
{
    std::queue<std::unique_ptr<Event>> eventsToReemit;
    std::unique_ptr<Event> event;

    while (world.poll(event))
    {
        if (event->getType() == "SpawnEnemyEvent")
        {
            // ✅ Delegación a EntityManager
            auto *spawnEvent = static_cast<SpawnEnemyEvent *>(event.get());
            EntityManager::CreateEnemy(
                world,
                m_Renderer,
                m_EnemyTypes,
                spawnEvent->enemyTypeName,
                spawnEvent->speedMultiplier);
        }
        else if (event->getType() == "ObstacleSpawnEvent")
        {
            // ✅ Delegación a EntityManager
            auto *obstacleEvent = static_cast<ObstacleSpawnEvent *>(event.get());
            EntityManager::CreateObstacle(
                world,
                m_Renderer,
                m_ObstacleConfig,
                obstacleEvent->x,
                obstacleEvent->y);
        }
        else
        {
            // Re-emitir eventos que no procesamos
            eventsToReemit.push(std::move(event));
        }
    }

    while (!eventsToReemit.empty())
    {
        world.emit(std::move(eventsToReemit.front()));
        eventsToReemit.pop();
    }
}
