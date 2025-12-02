#include "Game.h"
#include "ConfigLoader.h"
#include "Entity.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/HealthComponent.h"
#include "Components/EnemyComponent.h"
#include <glm/glm.hpp>

Game::Game()
    : m_Window(nullptr), m_Renderer(nullptr), m_IsRunning(false), m_GameOver(false), m_GameOverPrinted(false), m_World()
{
}

bool Game::Initialize()
{
    // ========================================
    // CARGAR CONFIGURACIÓN DESDE JSON
    // ========================================
    ConfigLoader config;
    if (!config.LoadSettings())
        return false;

    if (!config.LoadEnemyConfig())
        return false;

    // Cargar configuración de oleadas
    if (!config.LoadWavesConfig())
    {
        spdlog::error("No se pudo cargar la configuración de oleadas");
        return false;
    }

    // Cargar configuración de obstáculos (unidad mínima + posiciones por fase)
    config.LoadObstacleConfig();
    config.LoadObstaclesByPhaseConfig();

    auto window = config.GetWindow();
    auto gameplay = config.GetGameplay();
    auto collision = config.GetCollision();
    const auto &enemyTypes = config.GetEnemyEntity().enemyTypes; // Vector de structs tipadas

    m_Ancho = window.width;
    m_Alto = window.height;

    // ========================================
    // INICIALIZAR SDL
    // ========================================
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::error("SDL_Init error: {}", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow(window.title.c_str(), static_cast<int>(m_Ancho), static_cast<int>(m_Alto), SDL_WINDOW_BORDERLESS);
    if (!m_Window)
    {
        spdlog::error("SDL_CreateWindow error: {}", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
    if (!m_Renderer)
    {
        spdlog::error("SDL_CreateRenderer error: {}", SDL_GetError());
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
        return false;
    }

    // ========================================
    // INICIALIZAR SISTEMAS ECS
    // ========================================
    GameContext ctx(m_Renderer, m_Ancho, m_Alto); // solucion de long parameter list

    m_PlayerInputSystem = std::make_unique<PlayerInputSystem>(ctx, gameplay.playerSpeed);

    m_MovementSystem = std::make_unique<MovementSystem>();

    m_CollisionSystem = std::make_unique<CollisionSystem>(ctx);

    m_CollisionResponseSystem = std::make_unique<CollisionResponseSystem>(ctx, collision.playerPushStrength, collision.enemyPushStrength);

    m_RenderSystem = std::make_unique<RenderSystem>(ctx);

    m_DamageSystem = std::make_unique<DamageSystem>(&m_IsRunning, &m_GameOver, collision.playerInvulnerabilityDuration);

    m_EnemyAISystem = std::make_unique<EnemyAISystem>();

    m_HUDSystem = std::make_unique<HUDSystem>(ctx);

    // ✅ WaveManagerSystem: Orquesta WaveSystem, SpawnSystem, LifetimeSystem
    m_WaveManagerSystem = std::make_unique<WaveManagerSystem>(ctx, enemyTypes);

    // Cargar configuración de oleadas (que ya incluyen imagen de fondo)
    m_WaveManagerSystem->LoadWavesConfig(config.GetWaves().waves);

    // Cargar configuración de obstáculos
    m_WaveManagerSystem->LoadObstacleConfig(config.GetObstacle());
    m_WaveManagerSystem->LoadObstaclesByPhase(config.GetObstaclesByPhase());

    m_WaveManagerSystem->StartWaves(m_World);

    // ========================================
    // INICIALIZAR SISTEMA DE MÚSICA
    // ========================================
    m_MusicSystem = std::make_unique<MusicSystem>();
    if (!m_MusicSystem->LoadMusic("assets/song/Behind-The-Scene.wav"))
    {
        spdlog::warn("No se pudo cargar la música (continuando sin audio)");
        // No es crítico - el juego puede continuar sin música
    }

    spdlog::info("Sistemas ECS inicializados correctamente");
    m_IsRunning = true;
    return true;
}

bool Game::Start()
{
    // ========================================
    // CARGAR CONFIGURACIÓN DEL JUGADOR
    // ========================================
    ConfigLoader config;
    if (!config.LoadPlayerConfig())
        return false;

    // ✅ Calcular posición de spawn basada en dimensiones del mundo
    config.CalculatePlayerSpawnPosition(m_Ancho, m_Alto);
    auto playerCfg = config.GetPlayerEntity();

    // ========================================
    // CREAR EL JUGADOR
    // ========================================
    Entity &player = m_World.createEntity();

    auto playerComp = std::make_unique<PlayerComponent>();
    player.AddComponent(std::move(playerComp));

    //  Usar posición precalculada del config
    auto playerTransform = std::make_unique<TransformComponent>(playerCfg.spawnX, playerCfg.spawnY, 0.0f, 0.0f);
    player.AddComponent(std::move(playerTransform));

    auto playerSprite = std::make_unique<SpriteComponent>(playerCfg.spritePath, m_Renderer);
    player.AddComponent(std::move(playerSprite));

    auto playerCollider = std::make_unique<ColliderComponent>(playerCfg.colliderWidth, playerCfg.colliderHeight);
    player.AddComponent(std::move(playerCollider));

    auto playerHealth = std::make_unique<HealthComponent>(playerCfg.maxHp);
    player.AddComponent(std::move(playerHealth));

    spdlog::info("Jugador creado: ID={}, Posicion=({:.1f},{:.1f}), HP={}",
                 player.m_Id, playerCfg.spawnX, playerCfg.spawnY, playerCfg.maxHp);

    // ========================================
    // INICIAR MÚSICA
    // ========================================
    if (m_MusicSystem)
    {
        m_MusicSystem->Play();
    }

    return true;
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            m_IsRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat)
            {
                // Si está en Game Over, Enter o ESC salen
                if (m_GameOver)
                {
                    if (event.key.key == SDLK_RETURN || event.key.key == SDLK_ESCAPE)
                    {
                        m_IsRunning = false;
                    }
                }
                else
                {
                    // Durante el juego, solo ESC sale
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        m_IsRunning = false;
                    }
                }
            }
            break;
        }
    }
}

void Game::Update(float deltaTime)
{
    // Si está en Game Over, no actualizar la lógica del juego
    if (m_GameOver)
        return;

    // ========================================
    // FASE 1: ORQUESTACIÓN (Wave Management)
    // ========================================
    // ✅ WaveManagerSystem::update() emite eventos desde WaveSystem
    m_WaveManagerSystem->update(m_World, deltaTime);

    // ========================================
    // FASE 2: INPUT & AI
    // ========================================
    m_PlayerInputSystem->update(m_World, deltaTime);
    m_EnemyAISystem->update(m_World, deltaTime);

    // ========================================
    // FASE 3: FÍSICA (Movement & Collision)
    // ========================================
    m_MovementSystem->update(m_World, deltaTime);
    m_CollisionSystem->update(m_World, deltaTime);
    m_CollisionResponseSystem->update(m_World, deltaTime);

    // ========================================
    // FASE 4: DAÑO & LÓGICA DE JUEGO
    // ========================================
    m_DamageSystem->update(m_World, deltaTime);
}

void Game::Render(float deltaTime)
{
    if (m_GameOver)
    {
        // Detener música cuando el juego termina (SOLO UNA VEZ)
        if (!m_GameOverPrinted && m_MusicSystem)
        {
            m_MusicSystem->Stop();
        }

        // Renderizar pantalla de Game Over
        SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255); // Fondo negro
        SDL_RenderClear(m_Renderer);

        // Mostrar tiempo final SOLO UNA VEZ
        if (!m_GameOverPrinted)
        {
            float finalTime = m_RenderSystem->getElapsedTime();

            spdlog::info("=== GAME OVER ===");
            spdlog::info("Tiempo Final: {:.2f} segundos", finalTime);
            spdlog::info("Presiona ENTER o ESC para salir");

            m_GameOverPrinted = true; // Marcar como impreso
        }

        SDL_RenderPresent(m_Renderer);
    }
    else
    {
        // ========================================
        // LIMPIAR PANTALLA CON COLOR DE FONDO
        // ========================================
        SDL_SetRenderDrawColor(m_Renderer, 50, 50, 50, 255); // Gris oscuro
        SDL_RenderClear(m_Renderer);

        // ========================================
        // FASE 1: RENDERIZAR FONDO (desde WaveSystem)
        // ========================================
        m_WaveManagerSystem->render(m_World);

        // ========================================
        // FASE 2: RENDERIZAR CAPAS
        // ========================================
        // Capa 2: Sprites de entidades
        m_RenderSystem->update(m_World, deltaTime);

        // Capa 3: HUD (interfaz de usuario)
        m_HUDSystem->update(m_World, deltaTime);

        // Presentar frame completo
        SDL_RenderPresent(m_Renderer);
    }
}

void Game::Run()
{
    auto millisecsPreviousFrame = SDL_GetTicks();
    while (m_IsRunning)
    {
        ProcessInput();
        double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

        Update(deltaTime);
        millisecsPreviousFrame = SDL_GetTicks();
        Render(static_cast<float>(deltaTime));
    }
}

Game::~Game()
{
    if (m_Renderer)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    SDL_Quit();
}
