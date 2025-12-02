#pragma once
#include <string>
#include <json.hpp>

using json = nlohmann::json;

/**
 * @class ConfigLoader
 * @brief Carga y valida toda la configuración del juego desde archivos JSON
 *
 * Responsabilidad única: Leer JSON y devolver estructuras de datos validadas.
 */
class ConfigLoader
{
public:
    // ========================================
    // ESTRUCTURAS DE CONFIGURACIÓN
    // ========================================

    struct WindowConfig
    {
        float width = 1920.0f;
        float height = 1080.0f;
        std::string title = "Mega Man ECS Engine";
    };

    struct GameplayConfig
    {
        float playerSpeed = 400.0f;
    };

    struct CollisionConfig
    {
        float playerPushStrength = 10.0f;
        float enemyPushStrength = 10.0f;
        float playerInvulnerabilityDuration = 3.0f;
    };

    struct PlayerEntityConfig
    {
        std::string spritePath;
        float colliderWidth = 64.0f;
        float colliderHeight = 64.0f;
        int maxHp = 10;
        float spawnXOffset = 0.5f;
        float spawnYOffset = 0.5f;

        // ✅ Helper: Calcular posición de spawn
        void CalculateSpawnPosition(float worldWidth, float worldHeight)
        {
            spawnX = worldWidth * spawnXOffset - colliderWidth / 2.0f;
            spawnY = worldHeight * spawnYOffset - colliderHeight / 2.0f;
        }

        float spawnX = 960.0f; // Calculado al cargar config
        float spawnY = 540.0f; // Calculado al cargar config
    };

    /**
     * @brief Configuración de un tipo de enemigo (parseado una sola vez)
     */
    struct EnemyTypeConfig
    {
        std::string name;
        std::string spritePath;
        float colliderWidth;
        float colliderHeight;
        float baseSpeed;
        float focusRange;
        float velocitySmoothing;
        float lifetime;
    };

    /**
     * @brief Configuración completa de enemigos (vector de tipos)
     */
    struct EnemyEntityConfig
    {
        std::vector<EnemyTypeConfig> enemyTypes;
    };
    /**
     * @brief Configuración de un fondo por fase
     */

    /**
     * @brief Configuración de una oleada (wave)
     */
    struct WaveConfig
    {
        struct EnemySpawn
        {
            std::string type; // Nombre del tipo de enemigo (e.g., "Enemigo1")
            int count;        // Cantidad a spawner
        };

        int waveNumber;
        int phaseId;                     // ID de la fase de fondo asociada
        std::string imagePath;           // Ruta a la imagen de fondo para esta onda
        std::vector<EnemySpawn> enemies; // Lista de enemigos con cantidades
        float spawnInterval;             // Intervalo entre spawns
        float duration;                  // Duración total de la oleada
        float speedMultiplier;           // Multiplicador de velocidad
    };

    /**
     * @brief Configuración completa de oleadas
     */
    struct WavesData
    {
        std::vector<WaveConfig> waves;
    };

    /**
     * @brief Configuración de la unidad mínima de obstáculo
     */
    struct ObstacleConfig
    {
        std::string spritePath;
        float colliderWidth;
        float colliderHeight;
    };

    /**
     * @brief Posición de un obstáculo en una fase
     */
    struct ObstaclePosition
    {
        float x;
        float y;
    };

    /**
     * @brief Obstáculos de una fase
     */
    struct ObstaclePhaseData
    {
        std::string description; // Descripción de la fase
        std::vector<ObstaclePosition> obstaculos;
    };

    /**
     * @brief Configuración completa de obstáculos por fase
     */
    struct ObstaclesByPhaseData
    {
        std::vector<ObstaclePhaseData> phases; // 0 a 4 (5 fases)
    };

    // ========================================
    // MÉTODOS PÚBLICOS
    // ========================================

    /**
     * @brief Carga settings.json
     * @param path Ruta al archivo JSON (default: "./assets/config/settings.json")
     * @return true si se cargó correctamente
     */
    bool LoadSettings(const std::string &path = "./assets/config/settings.json");

    /**
     * @brief Carga Player_entity.json
     * @param path Ruta al archivo JSON (default: "./assets/config/Player_entity.json")
     * @return true si se cargó correctamente
     */
    bool LoadPlayerConfig(const std::string &path = "./assets/config/Player_entity.json");

    /**
     * @brief Carga Enemy_entities.json
     * @param path Ruta al archivo JSON (default: "./assets/config/Enemy_entities.json")
     * @return true si se cargó correctamente
     */
    bool LoadEnemyConfig(const std::string &path = "./assets/config/Enemy_entities.json");

    /**
     * @brief Carga Waves.json
     * @param path Ruta al archivo JSON (default: "./assets/config/Waves.json")
     * @return true si se cargó correctamente
     */
    bool LoadWavesConfig(const std::string &path = "./assets/config/Waves.json");

    /**
     * @brief Carga Obstaculo.json (unidad mínima de obstáculo)
     * @param path Ruta al archivo JSON (default: "./assets/config/Obstaculo.json")
     * @return true si se cargó correctamente
     */
    bool LoadObstacleConfig(const std::string &path = "./assets/config/Obstaculo.json");

    /**
     * @brief Carga ObstaculosByFase.json (posiciones de obstáculos por fase)
     * @param path Ruta al archivo JSON (default: "./assets/config/ObstaculosByFase.json")
     * @return true si se cargó correctamente
     */
    bool LoadObstaclesByPhaseConfig(const std::string &path = "./assets/config/ObstaculosByFase.json");

    /**
     * @brief Calcula la posición de spawn del jugador basada en dimensiones de mundo
     * @param worldWidth Ancho del mundo
     * @param worldHeight Alto del mundo
     */
    void CalculatePlayerSpawnPosition(float worldWidth, float worldHeight);

    // Getters para acceder a la configuración cargada
    const WindowConfig &GetWindow() const { return m_Window; }
    const GameplayConfig &GetGameplay() const { return m_Gameplay; }
    const CollisionConfig &GetCollision() const { return m_Collision; }
    const PlayerEntityConfig &GetPlayerEntity() const { return m_PlayerEntity; }
    const EnemyEntityConfig &GetEnemyEntity() const { return m_EnemyEntity; }
    const WavesData &GetWaves() const { return m_Waves; }
    const ObstacleConfig &GetObstacle() const { return m_Obstacle; }
    const ObstaclesByPhaseData &GetObstaclesByPhase() const { return m_ObstaclesByPhase; }

private:
    WindowConfig m_Window;
    GameplayConfig m_Gameplay;
    CollisionConfig m_Collision;
    PlayerEntityConfig m_PlayerEntity;
    EnemyEntityConfig m_EnemyEntity;
    WavesData m_Waves;
    ObstacleConfig m_Obstacle;
    ObstaclesByPhaseData m_ObstaclesByPhase;
};
