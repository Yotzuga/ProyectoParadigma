#include "../../include/Systems/WaveSystem.h"
#include "../../include/Entity.h"
#include "../../include/Event.h"
#include "../../include/Components/EnemyComponent.h"
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

WaveSystem::WaveSystem(SDL_Renderer *renderer, float screenWidth, float screenHeight,
                       const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes)
    : m_Renderer(renderer), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight),
      m_EnemyTypes(enemyTypes),
      m_CurrentWave(0), m_WaveTimer(0.0f), m_SpawnTimer(0.0f),
      m_EnemiesSpawnedInWave(0), m_WaveInProgress(false), m_WaveEnded(false),
      m_CurrentBackgroundTexture(nullptr)
{
}

WaveSystem::~WaveSystem()
{
    // Liberar textura de fondo
    if (m_CurrentBackgroundTexture)
    {
        SDL_DestroyTexture(m_CurrentBackgroundTexture);
        m_CurrentBackgroundTexture = nullptr;
    }
}

bool WaveSystem::LoadWavesConfig(const std::vector<ConfigLoader::WaveConfig> &waves)
{
    if (waves.empty())
    {
        spdlog::error("WaveSystem: No se proporcionaron configuraciones de oleadas");
        return false;
    }

    m_Waves.clear();

    for (const auto &waveConfig : waves)
    {
        WaveData wave;
        wave.waveNumber = waveConfig.waveNumber;
        wave.phaseId = waveConfig.phaseId;
        wave.imagePath = waveConfig.imagePath;
        wave.enemies = waveConfig.enemies; // ✅ Copiar lista de enemigos con cantidades
        wave.spawnInterval = waveConfig.spawnInterval;
        wave.duration = waveConfig.duration;
        wave.speedMultiplier = waveConfig.speedMultiplier;

        m_Waves.push_back(wave);
    }

    spdlog::info("WaveSystem: {} oleadas cargadas", m_Waves.size());
    return true;
}

void WaveSystem::LoadObstaclesByPhase(const ConfigLoader::ObstaclesByPhaseData &obstacles)
{
    m_ObstaclesByPhase = obstacles;
}

void WaveSystem::renderBackground()
{
    if (!m_Renderer || m_Waves.empty() || m_CurrentWave >= m_Waves.size())
        return;

    const auto &currentWave = m_Waves[m_CurrentWave];

    // Si ya no hay textura cargada o cambió la onda, cargar la nueva
    if (!m_CurrentBackgroundTexture)
    {
        if (!currentWave.imagePath.empty())
        {
            m_CurrentBackgroundTexture = IMG_LoadTexture(m_Renderer, currentWave.imagePath.c_str());

            if (!m_CurrentBackgroundTexture)
            {
                spdlog::warn("WaveSystem: No se pudo cargar imagen: {}", currentWave.imagePath);
            }
        }
    }

    // Renderizar la textura
    if (m_CurrentBackgroundTexture)
    {
        SDL_FRect bgRect{0.0f, 0.0f,
                         static_cast<float>(m_ScreenWidth),
                         static_cast<float>(m_ScreenHeight)};
        SDL_RenderTexture(m_Renderer, m_CurrentBackgroundTexture, nullptr, &bgRect);
    }
}

void WaveSystem::StartWave(World &world)
{
    if (m_CurrentWave >= m_Waves.size())
    {
        spdlog::warn("WaveSystem: Intento de iniciar ola inválida: {}", m_CurrentWave);
        return;
    }

    m_WaveInProgress = true;
    m_WaveTimer = 0.0f;
    m_SpawnTimer = 0.0f;
    m_EnemiesSpawnedInWave = 0;

    const auto &currentWave = m_Waves[m_CurrentWave];

    // ✅ Descargar textura anterior y cargar la nueva
    if (m_CurrentBackgroundTexture)
    {
        SDL_DestroyTexture(m_CurrentBackgroundTexture);
        m_CurrentBackgroundTexture = nullptr;
    }

    // La textura se cargará en renderBackground() cuando sea necesario

    spdlog::info("=== OLA {} INICIADA ===", m_CurrentWave + 1);

    // ✅ Emitir ObstacleSpawnEvent para TODOS los obstáculos de la fase actual, al mismo tiempo
    if (m_CurrentWave < m_ObstaclesByPhase.phases.size())
    {
        const auto &phaseObstacles = m_ObstaclesByPhase.phases[m_CurrentWave];
        for (const auto &obstaclePos : phaseObstacles.obstaculos)
        {
            auto obstacleEvent = std::make_unique<ObstacleSpawnEvent>(obstaclePos.x, obstaclePos.y);
            world.emit(std::move(obstacleEvent));
        }
    }
}

void WaveSystem::EndWave(World &world)
{
    m_WaveInProgress = false;
    m_WaveEnded = true; // ✅ Marcar que la oleada terminó para procesar en el siguiente frame
    spdlog::info("=== OLA {} FINALIZADA ===", m_CurrentWave + 1);

    // EMIT WaveEndEvent para que otros sistemas reaccionen (matar enemigos)
    auto waveEndEvent = std::make_unique<WaveEndEvent>(m_CurrentWave + 1);
    world.emit(std::move(waveEndEvent));

    // ⚠️ NO pasar a la siguiente oleada en este mismo frame
    // Se hará en el siguiente frame después de que LifetimeSystem procese el WaveEndEvent
}

void WaveSystem::NextWave(World &world)
{
    if (m_CurrentWave < m_Waves.size() - 1)
    {
        m_CurrentWave++;

        // La emisión de WaveStartEvent ahora se hace dentro de StartWave
        StartWave(world);
    }
    else
    {
        spdlog::info("=== TODAS LAS OLEADAS COMPLETADAS ===");
        m_WaveInProgress = false;
    }
}

void WaveSystem::update(World &world, float dt)
{
    // ✅ PRIMERO: Procesar si la oleada anterior fue marcada como terminada
    // Esto ocurre en el siguiente frame después de que LifetimeSystem haya procesado el WaveEndEvent
    if (m_WaveEnded)
    {
        m_WaveEnded = false;
        NextWave(world);
    }

    if (!m_WaveInProgress || m_Waves.empty())
        return;

    const auto &currentWave = m_Waves[m_CurrentWave];

    // Actualizar timer de la ola
    m_WaveTimer += dt;
    m_SpawnTimer += dt;

    // ✅ Calcular total de enemigos a spawner en esta ola
    int totalEnemiesToSpawn = 0;
    for (const auto &spawn : currentWave.enemies)
    {
        totalEnemiesToSpawn += spawn.count;
    }

    // Spawner enemigos según intervalo
    if (m_EnemiesSpawnedInWave < totalEnemiesToSpawn &&
        m_SpawnTimer >= currentWave.spawnInterval)
    {
        // Encontrar qué tipo de enemigo spawner basado en el contador
        int enemyIndex = 0;
        std::string enemyTypeToSpawn;

        for (const auto &spawn : currentWave.enemies)
        {
            if (enemyIndex + spawn.count > m_EnemiesSpawnedInWave)
            {
                enemyTypeToSpawn = spawn.type;
                break;
            }
            enemyIndex += spawn.count;
        }

        if (!enemyTypeToSpawn.empty())
        {
            // ✅ EMIT SpawnEnemyEvent con multiplicador de velocidad
            auto spawnEvent = std::make_unique<SpawnEnemyEvent>(enemyTypeToSpawn, currentWave.speedMultiplier);
            world.emit(std::move(spawnEvent));
        }

        m_EnemiesSpawnedInWave++;
        m_SpawnTimer = 0.0f;
    }

    // Verificar si la ola ha terminado (tiempo excedido)
    if (m_WaveTimer >= currentWave.duration)
    {
        EndWave(world);
    }
}
