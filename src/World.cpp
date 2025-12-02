#include "World.h"
#include "Entity.h"
#include "Components/PlayerComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/ObstacleComponent.h"
#include <algorithm>
#include <spdlog/spdlog.h>

World::World() = default;

Entity &World::createEntity()
{
    uint32_t id = m_NextEntityId++;
    std::string name = "Entity_" + std::to_string(id);
    auto entity = std::make_unique<Entity>(id, name);
    Entity *entityPtr = entity.get();
    m_Entities.push_back(std::move(entity));
    m_EntityMap[id] = entityPtr; // Agregar al mapa para búsqueda O(1)
    m_CacheDirty = true;
    return *entityPtr;
}

void World::deleteEntity(uint32_t id)
{
    auto mapIt = m_EntityMap.find(id);
    if (mapIt == m_EntityMap.end())
        return; // Entidad no existe

    Entity *entityPtr = mapIt->second;

    // Encontrar y eliminar del vector
    auto it = std::find_if(m_Entities.begin(), m_Entities.end(),
                           [entityPtr](const std::unique_ptr<Entity> &e)
                           { return e.get() == entityPtr; });
    if (it != m_Entities.end())
    {
        m_Entities.erase(it);
        m_EntityMap.erase(mapIt); // Remover del mapa
        m_CacheDirty = true;
    }
}

// Emite evento al bus
void World::emit(std::unique_ptr<Event> event)
{
    m_EventQueue.push(std::move(event));
}

// Obtiene siguiente evento de la cola
bool World::poll(std::unique_ptr<Event> &out)
{
    if (m_EventQueue.empty())
        return false;

    out = std::move(m_EventQueue.front());
    m_EventQueue.pop();
    return true;
}

World::~World()
{
    m_Entities.clear();
    while (!m_EventQueue.empty())
        m_EventQueue.pop();
}

// Reconstruye cachés solo cuando hay cambios
void World::RebuildCacheIfNeeded()
{
    if (!m_CacheDirty)
        return;

    m_EnemyEntities.clear();
    m_ObstacleEntities.clear();

    // Saltar m_Entities[0] (siempre es el jugador)
    for (size_t i = 1; i < m_Entities.size(); ++i)
    {
        Entity *e = m_Entities[i].get();

        if (e->GetComponent<EnemyComponent>())
            m_EnemyEntities.push_back(e);

        if (e->GetComponent<ObstacleComponent>())
            m_ObstacleEntities.push_back(e);
    }

    m_CacheDirty = false;
}

// Obtener enemigos
const std::vector<Entity *> &World::GetEnemies()
{
    RebuildCacheIfNeeded();
    return m_EnemyEntities;
}

// Obtener obstáculos
const std::vector<Entity *> &World::GetObstacles()
{
    RebuildCacheIfNeeded();
    return m_ObstacleEntities;
}

// Buscar entidad por ID (O(1) - HashMap)
Entity *World::FindEntityById(uint32_t id) const
{
    auto it = m_EntityMap.find(id);
    if (it != m_EntityMap.end())
        return it->second;
    return nullptr;
}
