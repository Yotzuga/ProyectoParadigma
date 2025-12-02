#include "ConfigLoader.h"
#include <fstream>
#include <spdlog/spdlog.h>

bool ConfigLoader::LoadSettings(const std::string &path)
{
    try
    {
        std::ifstream configFile(path);
        if (!configFile.is_open())
        {
            spdlog::error("No se pudo abrir {}", path);
            return false;
        }

        json settings = json::parse(configFile);

        // Cachear objetos intermedios para evitar búsquedas redundantes en JSON
        auto windowObj = settings.value("window", json::object());
        m_Window.width = windowObj["width"];
        m_Window.height = windowObj["height"];
        m_Window.title = windowObj.value("title", "Mega Man ECS Engine");

        auto gameplayObj = settings.value("gameplay", json::object());
        m_Gameplay.playerSpeed = gameplayObj["player_speed"];

        auto collisionObj = settings.value("collision", json::object());
        m_Collision.playerPushStrength = collisionObj.value("player_push_strength", 10.0f);
        m_Collision.enemyPushStrength = collisionObj.value("enemy_push_strength", 10.0f);
        m_Collision.playerInvulnerabilityDuration = collisionObj.value("player_invulnerability_duration", 3.0f);

        spdlog::info("=== SETTINGS CARGADOS ===");
        spdlog::info("Window: {}x{}, Title: {}", (int)m_Window.width, (int)m_Window.height, m_Window.title);
        spdlog::info("Gameplay: PlayerSpeed={:.1f}", m_Gameplay.playerSpeed);
        spdlog::info("Collision: playerPush={:.1f}, enemyPush={:.1f}, invulnerability={:.1f}s",
                     m_Collision.playerPushStrength,
                     m_Collision.enemyPushStrength, m_Collision.playerInvulnerabilityDuration);

        return true;
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error al leer {}: {}", path, e.what());
        return false;
    }
}

bool ConfigLoader::LoadPlayerConfig(const std::string &path)
{
    try
    {
        std::ifstream playerFile(path);
        if (!playerFile.is_open())
        {
            spdlog::error("No se pudo abrir {}", path);
            return false;
        }

        json playerConfig = json::parse(playerFile);

        // ✅ Cachear objetos intermedios para evitar búsquedas redundantes
        auto spriteObj = playerConfig.value("sprite", json::object());
        m_PlayerEntity.spritePath = spriteObj["path"];

        auto colliderObj = playerConfig.value("collider", json::object());
        m_PlayerEntity.colliderWidth = colliderObj["width"];
        m_PlayerEntity.colliderHeight = colliderObj["height"];

        auto healthObj = playerConfig.value("health", json::object());
        m_PlayerEntity.maxHp = healthObj["max_hp"];

        auto spawnObj = playerConfig.value("spawn", json::object());
        m_PlayerEntity.spawnXOffset = spawnObj.value("x_offset", 0.5f);
        m_PlayerEntity.spawnYOffset = spawnObj.value("y_offset", 0.5f);

        spdlog::info("=== PLAYER CONFIG CARGADA ===");
        spdlog::info("Sprite: {}", m_PlayerEntity.spritePath);
        spdlog::info("Collider: {}x{}", m_PlayerEntity.colliderWidth, m_PlayerEntity.colliderHeight);
        spdlog::info("Max HP: {}", m_PlayerEntity.maxHp);

        return true;
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error al leer {}: {}", path, e.what());
        return false;
    }
}

bool ConfigLoader::LoadEnemyConfig(const std::string &path)
{
    try
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            spdlog::error("ConfigLoader: No se pudo abrir {}", path);
            return false;
        }

        json enemyJson = json::parse(file);

        if (!enemyJson.contains("enemies") || !enemyJson["enemies"].is_array())
        {
            spdlog::error("ConfigLoader: Enemy_entities.json no contiene array 'enemies'");
            return false;
        }

        m_EnemyEntity.enemyTypes.clear();

        for (const auto &enemyData : enemyJson["enemies"])
        {
            EnemyTypeConfig enemyType;

            enemyType.name = enemyData["name"].get<std::string>();
            enemyType.spritePath = enemyData["sprite"]["path"].get<std::string>();
            enemyType.colliderWidth = enemyData["collider"]["width"].get<float>();
            enemyType.colliderHeight = enemyData["collider"]["height"].get<float>();
            enemyType.baseSpeed = enemyData["ai"]["base_speed"].get<float>();
            enemyType.focusRange = enemyData["ai"]["focus_range"].get<float>();
            enemyType.velocitySmoothing = enemyData["ai"]["velocity_smoothing"].get<float>();
            enemyType.lifetime = enemyData.value("lifetime", 0.0f);

            m_EnemyEntity.enemyTypes.push_back(enemyType);
        }

        spdlog::info("=== ENEMY CONFIG CARGADA ===");
        spdlog::info("Tipos de enemigos disponibles: {}", m_EnemyEntity.enemyTypes.size());

        return true;
    }
    catch (const json::parse_error &e)
    {
        spdlog::error("ConfigLoader: Error al parsear {}: {}", path, e.what());
        return false;
    }
    catch (const std::exception &e)
    {
        spdlog::error("ConfigLoader: Error inesperado al cargar {}: {}", path, e.what());
        return false;
    }
}

bool ConfigLoader::LoadWavesConfig(const std::string &path)
{
    try
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            spdlog::error("ConfigLoader: No se pudo abrir {}", path);
            return false;
        }

        json wavesJson = json::parse(file);

        if (!wavesJson.contains("waves") || !wavesJson["waves"].is_array())
        {
            spdlog::error("ConfigLoader: Waves.json no contiene array 'waves'");
            return false;
        }

        m_Waves.waves.clear();

        for (const auto &waveData : wavesJson["waves"])
        {
            WaveConfig wave;

            wave.waveNumber = waveData.value("wave_number", 0);
            wave.phaseId = waveData.value("phase_id", 0);
            wave.imagePath = waveData.value("image", "");
            wave.spawnInterval = waveData.value("spawn_interval", 1.0f);
            wave.duration = waveData.value("duration", 30.0f);
            wave.speedMultiplier = waveData.value("increase_based_speed", 1.0f);

            // ✅ Parsear array de objetos enemigos { "type": "Enemigo1", "count": 3 }
            auto enemiesArray = waveData.value("enemies", json::array());
            if (enemiesArray.is_array())
            {
                for (const auto &enemyObj : enemiesArray)
                {
                    if (enemyObj.contains("type") && enemyObj.contains("count"))
                    {
                        WaveConfig::EnemySpawn spawn;
                        spawn.type = enemyObj["type"].get<std::string>();
                        spawn.count = enemyObj["count"].get<int>();
                        wave.enemies.push_back(spawn);
                    }
                }
            }

            m_Waves.waves.push_back(wave);
        }

        spdlog::info("=== WAVES CONFIG CARGADA ===");
        spdlog::info("Oleadas disponibles: {}", m_Waves.waves.size());

        return true;
    }
    catch (const json::parse_error &e)
    {
        spdlog::error("ConfigLoader: Error al parsear {}: {}", path, e.what());
        return false;
    }
    catch (const std::exception &e)
    {
        spdlog::error("ConfigLoader: Error inesperado al cargar {}: {}", path, e.what());
        return false;
    }
}

void ConfigLoader::CalculatePlayerSpawnPosition(float worldWidth, float worldHeight)
{
    m_PlayerEntity.CalculateSpawnPosition(worldWidth, worldHeight);
}

bool ConfigLoader::LoadObstacleConfig(const std::string &path)
{
    try
    {
        std::ifstream obstacleFile(path);
        if (!obstacleFile.is_open())
        {
            spdlog::warn("No se pudo abrir {}", path);
            return false;
        }

        json config = json::parse(obstacleFile);
        auto obstacleObj = config.value("obstaculo", json::object());

        // Parsear sprite
        auto spriteObj = obstacleObj.value("sprite", json::object());
        m_Obstacle.spritePath = spriteObj["path"].get<std::string>();

        // Parsear collider
        auto colliderObj = obstacleObj.value("collider", json::object());
        m_Obstacle.colliderWidth = colliderObj["width"].get<float>();
        m_Obstacle.colliderHeight = colliderObj["height"].get<float>();

        spdlog::info("=== OBSTACLE CONFIG CARGADA ===");
        spdlog::info("Sprite: {}", m_Obstacle.spritePath);
        spdlog::info("Collider: {}x{}", (int)m_Obstacle.colliderWidth, (int)m_Obstacle.colliderHeight);

        return true;
    }
    catch (const json::parse_error &e)
    {
        spdlog::error("ConfigLoader: Error al parsear {}: {}", path, e.what());
        return false;
    }
    catch (const std::exception &e)
    {
        spdlog::error("ConfigLoader: Error inesperado al cargar {}: {}", path, e.what());
        return false;
    }
}

bool ConfigLoader::LoadObstaclesByPhaseConfig(const std::string &path)
{
    try
    {
        std::ifstream phasesFile(path);
        if (!phasesFile.is_open())
        {
            spdlog::warn("No se pudo abrir {}", path);
            return false;
        }

        json phases = json::parse(phasesFile);

        // Inicializar 5 fases (fase_0 a fase_4)
        m_ObstaclesByPhase.phases.resize(5);

        for (int phaseId = 0; phaseId < 5; ++phaseId)
        {
            std::string phaseKey = "fase_" + std::to_string(phaseId);

            if (!phases.contains(phaseKey))
            {
                continue;
            }

            auto phaseData = phases[phaseKey];

            // ✅ Cargar descripción de la fase
            m_ObstaclesByPhase.phases[phaseId].description = phaseData.value("description", "");

            auto obstaculosArray = phaseData.value("obstaculos", json::array());

            if (obstaculosArray.is_array())
            {
                for (const auto &obstacleObj : obstaculosArray)
                {
                    if (obstacleObj.contains("x") && obstacleObj.contains("y"))
                    {
                        ObstaclePosition pos;
                        pos.x = obstacleObj["x"].get<float>();
                        pos.y = obstacleObj["y"].get<float>();
                        m_ObstaclesByPhase.phases[phaseId].obstaculos.push_back(pos);
                    }
                }
            }

            spdlog::debug("ConfigLoader: Fase {} cargada - Descripción: '{}', {} obstáculos",
                          phaseId, m_ObstaclesByPhase.phases[phaseId].description,
                          m_ObstaclesByPhase.phases[phaseId].obstaculos.size());
        }

        spdlog::info("=== OBSTACLES BY PHASE CONFIG CARGADA ===");

        return true;
    }
    catch (const json::parse_error &e)
    {
        spdlog::error("ConfigLoader: Error al parsear {}: {}", path, e.what());
        return false;
    }
    catch (const std::exception &e)
    {
        spdlog::error("ConfigLoader: Error inesperado al cargar {}: {}", path, e.what());
        return false;
    }
}
