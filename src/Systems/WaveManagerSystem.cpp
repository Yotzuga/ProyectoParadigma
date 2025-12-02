#include "../../include/Systems/WaveManagerSystem.h"
#include "../../include/Game.h"
#include <spdlog/spdlog.h>

WaveManagerSystem::WaveManagerSystem(const GameContext &ctx,
                                     const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes)
    : m_Renderer(ctx.renderer),
      m_ScreenWidth(ctx.worldWidth),
      m_ScreenHeight(ctx.worldHeight)
{
    spdlog::info("WaveManagerSystem: Inicializando subsistemas...");

    // Crear subsistemas en orden de dependencia
    m_WaveSystem = std::make_unique<WaveSystem>(ctx.renderer, ctx.worldWidth, ctx.worldHeight, enemyTypes);
    m_SpawnSystem = std::make_unique<SpawnSystem>(ctx.renderer, ctx.worldWidth, ctx.worldHeight, enemyTypes);
    m_LifetimeSystem = std::make_unique<LifetimeSystem>();

    spdlog::info("WaveManagerSystem: Todos los subsistemas inicializados correctamente");
}

WaveManagerSystem::~WaveManagerSystem()
{
}

void WaveManagerSystem::update(World &world, float dt)
{
    // ✅ FASE DE LÓGICA: Procesar oleadas Y eventos de spawning
    // ORDEN: WaveSystem → SpawnSystem → LifetimeSystem
    // Esto asegura que los eventos se procesen en orden correcto

    // Paso 1: WaveSystem emite SpawnEnemyEvent, WaveStartEvent, WaveEndEvent
    if (m_WaveSystem)
    {
        m_WaveSystem->update(world, dt);
    }

    // Paso 2: SpawnSystem procesa SpawnEnemyEvent y crea enemigos
    // ⚠️ IMPORTANTE: Esto ocurre en el mismo frame que se emite el evento
    if (m_SpawnSystem)
    {
        m_SpawnSystem->update(world, dt);
    }

    // Paso 3: LifetimeSystem procesa WaveEndEvent y limpia enemigos Y obstáculos
    // Cuando una oleada termina, se eliminan todos los enemigos y obstáculos
    if (m_LifetimeSystem)
    {
        m_LifetimeSystem->update(world, dt);
    }
}

void WaveManagerSystem::render(World &world)
{
    // ✅ FASE DE RENDER: Solo renderizar fondos
    if (m_WaveSystem)
    {
        m_WaveSystem->renderBackground();
    }
}

bool WaveManagerSystem::LoadWavesConfig(const std::vector<ConfigLoader::WaveConfig> &waves)
{
    if (!m_WaveSystem)
    {
        spdlog::error("WaveManagerSystem: WaveSystem no inicializado");
        return false;
    }
    return m_WaveSystem->LoadWavesConfig(waves);
}

void WaveManagerSystem::LoadObstacleConfig(const ConfigLoader::ObstacleConfig &obstacle)
{
    if (!m_SpawnSystem)
    {
        spdlog::error("WaveManagerSystem: SpawnSystem no inicializado");
        return;
    }
    m_SpawnSystem->SetObstacleConfig(obstacle);
}

void WaveManagerSystem::LoadObstaclesByPhase(const ConfigLoader::ObstaclesByPhaseData &obstacles)
{
    if (!m_WaveSystem)
    {
        spdlog::error("WaveManagerSystem: WaveSystem no inicializado");
        return;
    }
    m_WaveSystem->LoadObstaclesByPhase(obstacles);
}

void WaveManagerSystem::StartWaves(World &world)
{
    if (!m_WaveSystem)
    {
        spdlog::error("WaveManagerSystem: WaveSystem no inicializado");
        return;
    }
    m_WaveSystem->StartWave(world);
}

int WaveManagerSystem::GetCurrentWave() const
{
    return m_WaveSystem ? m_WaveSystem->GetCurrentWave() : -1;
}

int WaveManagerSystem::GetTotalWaves() const
{
    return m_WaveSystem ? m_WaveSystem->GetTotalWaves() : 0;
}

bool WaveManagerSystem::IsWaveInProgress() const
{
    return m_WaveSystem ? m_WaveSystem->IsWaveInProgress() : false;
}
