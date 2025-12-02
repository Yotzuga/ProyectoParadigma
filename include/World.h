#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include "Entity.h"
#include "Event.h"
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>

/**
 * @class World
 * @brief Contenedor central de entidades y gestor de eventos.
 *
 * Responsabilidades:
 * - Crear y destruir entidades
 * - Mantener caches de Jugador, Enemigos, Obstáculos
 * - Cola de eventos para comunicación entre sistemas
 * - Búsqueda O(1) de entidades por ID (HashMap)
 *
 * Optimizaciones:
 * - HashMap m_EntityMap para O(1) lookup por ID
 * - Caches de Jugador/Enemigos/Obstáculos para O(1) iteración
 * - m_CacheDirty flag para reconstrucción lazy de caches
 */
class World
{
private:
    // ===== ALMACENAMIENTO DE ENTIDADES =====
    std::vector<std::unique_ptr<Entity>> m_Entities;    ///< Contenedor principal (dueño)
    std::unordered_map<uint32_t, Entity *> m_EntityMap; ///< Índice O(1) por ID (no propietario)
    uint32_t m_NextEntityId = 1;                        ///< Contador para IDs únicos

    // ===== CACHES OPTIMIZADAS =====
    std::vector<Entity *> m_EnemyEntities;    ///< Caché de enemigos (referencias, no propietario)
    std::vector<Entity *> m_ObstacleEntities; ///< Caché de obstáculos (referencias)
    bool m_CacheDirty = true;                 ///< Flag: ¿Caches necesitan reconstrucción?

    // ===== COLA DE EVENTOS =====
    std::queue<std::unique_ptr<Event>> m_EventQueue; ///< Cola de eventos central

    // ===== MÉTODOS PRIVADOS =====
    /**
     * @brief Reconstruye caches si m_CacheDirty es true
     * Busca componentes específicos para clasificar entidades
     */
    void RebuildCacheIfNeeded();

public:
    // ===== CONSTRUCTORES & DESTRUCTORES =====
    World();
    ~World();

    // ===== GESTIÓN DE ENTIDADES =====
    /**
     * @brief Crea una nueva entidad con ID único
     * @return Referencia a la entidad creada
     */
    Entity &createEntity();

    /**
     * @brief Elimina una entidad por ID
     * @param id ID de la entidad a eliminar
     */
    void deleteEntity(uint32_t id);

    // ===== ACCESO RÁPIDO A ENTIDADES =====
    /**
     * @brief Obtiene el jugador (primera entidad)
     * @return Puntero al jugador o nullptr si no existe
     */
    Entity *GetPlayer() const { return m_Entities.empty() ? nullptr : m_Entities[0].get(); }

    /**
     * @brief Obtiene caché de enemigos
     * @return Vector de referencias a entidades enemigo
     */
    const std::vector<Entity *> &GetEnemies();

    /**
     * @brief Obtiene caché de obstáculos
     * @return Vector de referencias a entidades obstáculo
     */
    const std::vector<Entity *> &GetObstacles();

    /**
     * @brief Obtiene todas las entidades
     * @return Vector const de todas las entidades
     */
    const std::vector<std::unique_ptr<Entity>> &GetAllEntities() const { return m_Entities; }

    /**
     * @brief Busca entidad por ID (O(1) HashMap)
     * @param id ID de la entidad
     * @return Puntero a la entidad o nullptr
     */
    Entity *FindEntityById(uint32_t id) const;

    // ===== GESTIÓN DE EVENTOS =====
    /**
     * @brief Emite un evento a la cola
     * @param event Evento a emitir
     */
    void emit(std::unique_ptr<Event> event);

    /**
     * @brief Procesa el siguiente evento de la cola
     * @param out Referencia para devolver el evento
     * @return true si hay evento, false si la cola está vacía
     */
    bool poll(std::unique_ptr<Event> &out);
};