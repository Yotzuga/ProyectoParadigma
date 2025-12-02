#pragma once
#include "../ISystem.h"
#include "../World.h"
#include "../ConfigLoader.h"
#include "WaveSystem.h"
#include "SpawnSystem.h"
#include "LifetimeSystem.h"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class WaveManagerSystem
 * @brief Orquestador central de oleadas y progresión de fases.
 *
 * Responsabilidades:
 * - Gestionar subsistemas: WaveSystem, SpawnSystem, LifetimeSystem
 * - Sincronizar timeline de oleadas
 * - Cargar y aplicar configuración de oleadas/obstáculos
 * - Iniciar y finalizar el juego
 *
 * Flujo de Oleadas:
 * 1. WaveSystem gestiona timeline y emite eventos (SpawnEnemy, WaveStart, WaveEnd)
 * 2. SpawnSystem escucha SpawnEnemyEvent y crea enemigos
 * 3. LifetimeSystem escucha WaveEndEvent y limpia enemigos/obstáculos
 * 4. WaveSystem renderiza fondos según fase
 *
 * Subsistemas internos:
 * - m_WaveSystem: Orquestación temporal + renderizado de fondos
 * - m_SpawnSystem: Creación de entidades dinámicas
 * - m_LifetimeSystem: Limpieza al fin de ola
 *
 * Nota: BackgroundSystem se integró en WaveSystem
 */
class WaveManagerSystem : public ISystem
{
private:
    // ===== SUBSISTEMAS INTERNOS =====
    std::unique_ptr<WaveSystem> m_WaveSystem;         ///< Gestor de oleadas + renderizado de fondos
    std::unique_ptr<SpawnSystem> m_SpawnSystem;       ///< Creador de enemigos y obstáculos
    std::unique_ptr<LifetimeSystem> m_LifetimeSystem; ///< Limpieza de entidades

    // ===== CONTEXTO SDL =====
    SDL_Renderer *m_Renderer; ///< Contexto de renderizado SDL3
    float m_ScreenWidth;      ///< Ancho de pantalla
    float m_ScreenHeight;     ///< Alto de pantalla

public:
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    /**
     * @brief Crea todos los subsistemas internos
     * @param ctx Contexto del juego (renderer, dimensions)
     * @param enemyTypes Vector de tipos de enemigos desde JSON
     */
    WaveManagerSystem(const GameContext &ctx,
                      const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes);

    /**
     * @brief Destruye todos los subsistemas
     */
    ~WaveManagerSystem();

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Fase de UPDATE: Procesa lógica de oleadas y eventos
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;

    /**
     * @brief Fase de RENDER: Renderiza fondos y procesa limpieza
     * @param world Referencia al mundo
     */
    void render(World &world);

    // ===== CONFIGURACIÓN =====
    /**
     * @brief Carga configuración de oleadas desde JSON
     * @param waves Vector de configuraciones de oleadas
     * @return true si carga exitosa
     */
    bool LoadWavesConfig(const std::vector<ConfigLoader::WaveConfig> &waves);

    /**
     * @brief Carga configuración mínima de obstáculos
     * @param obstacle Estructura con sprite y tamaño de colisión
     */
    void LoadObstacleConfig(const ConfigLoader::ObstacleConfig &obstacle);

    /**
     * @brief Carga posiciones de obstáculos por fase
     * @param obstacles Mapa de fase -> lista de obstáculos
     */
    void LoadObstaclesByPhase(const ConfigLoader::ObstaclesByPhaseData &obstacles);

    // ===== CONTROL DE JUEGO =====
    /**
     * @brief Inicia las oleadas
     * @param world Referencia al mundo
     */
    void StartWaves(World &world);

    // ===== CONSULTAS =====
    /**
     * @brief Obtiene número actual de oleada
     * @return Número de oleada (0-indexed)
     */
    int GetCurrentWave() const;

    /**
     * @brief Obtiene total de oleadas
     * @return Cantidad de oleadas definidas
     */
    int GetTotalWaves() const;

    /**
     * @brief Verifica si hay oleada en progreso
     * @return true si se está jugando una oleada
     */
    bool IsWaveInProgress() const;
};
