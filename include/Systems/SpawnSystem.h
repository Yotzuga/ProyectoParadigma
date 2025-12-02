#pragma once
#include "../ISystem.h"
#include "../World.h"
#include "../ConfigLoader.h"
#include <SDL3/SDL.h>
#include <vector>

/**
 * @class SpawnSystem
 * @brief Sistema de CREACIÓN de entidades.
 *
 * Responsabilidades:
 * - Escucha SpawnEnemyEvent y SpawnObstacleEvent
 * - Crea entidades (enemigos u obstáculos) en posiciones especificadas
 * - Utiliza EntityFactory para inicialización consistente
 * - Emite eventos de creación para logging
 *
 * Flujo:
 * 1. WaveSystem emite SpawnEnemyEvent
 * 2. SpawnSystem procesa evento
 * 3. Crea enemigo con EntityFactory::CreateEnemy
 * 4. Agregua entidad al World
 *
 * Configuración:
 * - Recibe tipos de enemigos desde JSON (velocidad, rango, etc.)
 * - Recibe configuración de obstáculos (tamaño de colisión)
 * - Genera posiciones aleatorias dentro del mundo
 *
 * Parámetros:
 * - worldWidth, worldHeight: Límites para spawn aleatorio
 * - m_EnemyTypes: Vector de configuraciones de enemigos
 * - m_ObstacleConfig: Configuración de obstáculos
 */
class SpawnSystem : public ISystem
{
private:
    // ===== CONTEXTO SDL & MUNDO =====
    SDL_Renderer *m_Renderer; ///< Renderer para crear sprites
    float m_WorldWidth;       ///< Ancho del mundo (para spawn aleatorio)
    float m_WorldHeight;      ///< Alto del mundo (para spawn aleatorio)

    // ===== CONFIGURACIÓN =====
    std::vector<ConfigLoader::EnemyTypeConfig> m_EnemyTypes; ///< Tipos de enemigos disponibles
    ConfigLoader::ObstacleConfig m_ObstacleConfig;           ///< Config de obstáculos

public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de spawn
     * @param renderer Contexto SDL3 renderer
     * @param worldWidth Ancho del mundo
     * @param worldHeight Alto del mundo
     * @param enemyTypes Vector de tipos de enemigos desde JSON
     */
    SpawnSystem(SDL_Renderer *renderer, float worldWidth, float worldHeight,
                const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Procesa eventos de spawn y crea entidades
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;

    // ===== CONFIGURACIÓN =====
    /**
     * @brief Asigna configuración de obstáculos
     * @param config Configuración (tamaño, sprite, etc.)
     */
    void SetObstacleConfig(const ConfigLoader::ObstacleConfig &config) { m_ObstacleConfig = config; }
};
