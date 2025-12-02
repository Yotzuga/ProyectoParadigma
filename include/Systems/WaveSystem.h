#pragma once
#include "../ISystem.h"
#include "../World.h"
#include "../ConfigLoader.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>

/**
 * @class WaveSystem
 * @brief Gestor de oleadas y renderizado de fondos.
 *
 * Responsabilidades:
 * - Mantener timeline de oleadas (duración, spawn rate)
 * - Emitir SpawnEnemyEvent en intervalo especificado
 * - Emitir WaveStartEvent y WaveEndEvent en transiciones
 * - Cargar y renderizar texturas de fondos por fase
 * - Gestionar transición a siguiente oleada
 *
 * Timeline de Oleada:
 * 1. Inicia oleada: emite WaveStartEvent
 * 2. Cada spawnInterval: emite SpawnEnemyEvent para cada enemigo pendiente
 * 3. Al alcanzar duration: emite WaveEndEvent y prepara transición
 * 4. Siguiente frame: avanza a siguiente oleada
 *
 * Renderizado:
 * - Carga textura de fondo según phaseId
 * - Renderiza fondo en cada frame
 * - Libera textura anterior al cambiar oleada
 *
 * Nota: WaveData contiene toda la configuración de una oleada
 *       (número, fase, fondo, enemigos, timing)
 */
class WaveSystem : public ISystem
{
private:
    // ===== ESTRUCTURA DE OLEADA =====
    /**
     * @struct WaveData
     * @brief Configuración completa de una oleada
     */
    struct WaveData
    {
        int waveNumber;                                            ///< Número de oleada (0-indexed)
        int phaseId;                                               ///< ID de fase (para obstáculos y fondos)
        std::string imagePath;                                     ///< Ruta de textura de fondo
        std::vector<ConfigLoader::WaveConfig::EnemySpawn> enemies; ///< Lista de enemigos a spawner
        float spawnInterval;                                       ///< Intervalo entre spawns (segundos)
        float duration;                                            ///< Duración total de la oleada
        float speedMultiplier;                                     ///< Multiplicador de velocidad
    };

    // ===== ALMACENAMIENTO =====
    std::vector<WaveData> m_Waves;                           ///< Todas las oleadas definidas
    std::vector<ConfigLoader::EnemyTypeConfig> m_EnemyTypes; ///< Tipos de enemigos disponibles
    ConfigLoader::ObstaclesByPhaseData m_ObstaclesByPhase;   ///< Obstáculos por fase

    // ===== RENDERIZADO =====
    SDL_Texture *m_CurrentBackgroundTexture; ///< Textura del fondo actual (propiedad)

    // ===== ESTADO DE OLEADA =====
    int m_CurrentWave;          ///< Índice de oleada actual
    float m_WaveTimer;          ///< Contador de tiempo en oleada actual (segundos)
    float m_SpawnTimer;         ///< Contador para siguiente spawn (segundos)
    int m_EnemiesSpawnedInWave; ///< Cantidad de enemigos ya spawnados en oleada
    bool m_WaveInProgress;      ///< ¿Hay oleada activa?
    bool m_WaveEnded;           ///< Flag: oleada terminó, esperar siguiente frame para avanzar

    // ===== CONTEXTO SDL =====
    SDL_Renderer *m_Renderer; ///< Contexto de renderizado SDL3
    float m_ScreenWidth;      ///< Ancho de pantalla
    float m_ScreenHeight;     ///< Alto de pantalla

public:
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    /**
     * @brief Inicializa el sistema de oleadas
     * @param renderer Contexto SDL3 renderer
     * @param screenWidth Ancho de pantalla
     * @param screenHeight Alto de pantalla
     * @param enemyTypes Vector de tipos de enemigos disponibles
     */
    WaveSystem(SDL_Renderer *renderer, float screenWidth, float screenHeight,
               const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes);

    /**
     * @brief Libera texturas de fondos
     */
    ~WaveSystem();

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Actualiza timeline de oleada y emite eventos
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;

    // ===== CONFIGURACIÓN =====
    /**
     * @brief Carga todas las oleadas desde JSON
     * @param waves Vector de configuraciones de oleadas
     * @return true si carga exitosa
     */
    bool LoadWavesConfig(const std::vector<ConfigLoader::WaveConfig> &waves);

    /**
     * @brief Carga obstáculos por fase
     * @param obstacles Mapa de phaseId -> lista de obstáculos
     */
    void LoadObstaclesByPhase(const ConfigLoader::ObstaclesByPhaseData &obstacles);

    // ===== RENDERIZADO =====
    /**
     * @brief Renderiza el fondo actual
     * Llamar desde WaveManagerSystem::render()
     */
    void renderBackground();

    // ===== CONTROL DE OLEADAS =====
    /**
     * @brief Inicia una oleada
     * @param world Referencia al mundo
     */
    void StartWave(World &world);

    /**
     * @brief Finaliza oleada actual
     * @param world Referencia al mundo
     */
    void EndWave(World &world);

    /**
     * @brief Avanza a siguiente oleada
     * @param world Referencia al mundo
     */
    void NextWave(World &world);

    // ===== CONSULTAS =====
    /**
     * @brief Obtiene número de oleada actual
     * @return Índice de oleada (0-indexed)
     */
    int GetCurrentWave() const { return m_CurrentWave; }

    /**
     * @brief Obtiene total de oleadas
     * @return Cantidad total de oleadas
     */
    int GetTotalWaves() const { return static_cast<int>(m_Waves.size()); }

    /**
     * @brief Verifica si hay oleada activa
     * @return true si se está jugando
     */
    bool IsWaveInProgress() const { return m_WaveInProgress; }
};
