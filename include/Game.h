#pragma once
#include <string>
#include <iostream>
#include "World.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
#include <memory>

// ========================================
// GameContext: Encapsula contexto global
// ========================================
/**
 * @struct GameContext
 * @brief Encapsula el contexto global del juego
 *
 * En lugar de pasar m_Renderer, m_Ancho, m_Alto por separado a cada sistema,
 * se empaca todo en una estructura única.
 */
struct GameContext
{
    SDL_Renderer *renderer = nullptr; // Contexto de renderizado SDL3
    float worldWidth = 0.0f;          // Ancho del mundo (píxeles)
    float worldHeight = 0.0f;         // Alto del mundo (píxeles)

    GameContext() = default;

    GameContext(SDL_Renderer *r, float w, float h)
        : renderer(r), worldWidth(w), worldHeight(h)
    {
    }
};

// Incluir los sistemas
#include "Systems/PlayerInputSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/CollisionResponseSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/EnemyAISystem.h"
#include "Systems/HUDSystem.h"
#include "Systems/WaveManagerSystem.h"

/**
 * @class Game
 * @brief Clase principal que orquesta la arquitectura ECS y el loop de juego.
 *
 * Responsabilidades:
 * - Inicialización de SDL y sistemas ECS
 * - Gestión del loop principal (ProcessInput → Update → Render)
 * - Orquestación del ciclo de vida del juego
 *
 * Arquitectura:
 * - PlayerInputSystem → Procesa input de teclado
 * - MovementSystem → Actualiza posiciones
 * - CollisionSystem → Detecta colisiones
 * - CollisionResponseSystem → Resuelve colisiones
 * - DamageSystem → Aplica daño
 * - EnemyAISystem → IA de enemigos
 * - RenderSystem → Renderiza sprites
 * - HUDSystem → Interfaz de usuario
 * - WaveManagerSystem → Orquesta ondas y obstáculos
 */
class Game
{
private:
    // ===== CONTEXTO SDL =====
    SDL_Window *m_Window;     ///< Ventana principal del juego
    SDL_Renderer *m_Renderer; ///< Contexto de renderizado SDL3
    float m_Ancho;            ///< Ancho de la ventana (píxeles)
    float m_Alto;             ///< Alto de la ventana (píxeles)

    // ===== ESTADO DEL JUEGO =====
    bool m_IsRunning;       ///< True mientras el juego está activo
    bool m_GameOver;        ///< True cuando el jugador muere
    bool m_GameOverPrinted; ///< Flag para imprimir Game Over una sola vez
    World m_World;          ///< Almacén central de entidades y eventos

    // ===== SISTEMAS ECS =====
    std::unique_ptr<PlayerInputSystem> m_PlayerInputSystem;             ///< Input del jugador
    std::unique_ptr<MovementSystem> m_MovementSystem;                   ///< Aplicar velocidad
    std::unique_ptr<RenderSystem> m_RenderSystem;                       ///< Renderizar sprites
    std::unique_ptr<CollisionSystem> m_CollisionSystem;                 ///< Detectar colisiones
    std::unique_ptr<CollisionResponseSystem> m_CollisionResponseSystem; ///< Resolver colisiones
    std::unique_ptr<DamageSystem> m_DamageSystem;                       ///< Sistema de daño
    std::unique_ptr<EnemyAISystem> m_EnemyAISystem;                     ///< IA de enemigos
    std::unique_ptr<HUDSystem> m_HUDSystem;                             ///< Interfaz de usuario

    // ===== MANAGER CENTRAL =====
    std::unique_ptr<WaveManagerSystem> m_WaveManagerSystem; ///< Orquesta: Waves, Background, Spawn, Lifetime

public:
    // ===== CICLO DE VIDA =====
    Game();
    ~Game();

    /**
     * @brief Inicializa SDL, crea ventana/renderer, carga configuración
     * @return true si la inicialización fue exitosa
     */
    bool Initialize();

    /**
     * @brief Crea el jugador y sus componentes
     * @return true si el jugador se creó correctamente
     */
    bool Start();

    /**
     * @brief Loop principal del juego (input → update → render)
     */
    void Run();

    // ===== ETAPAS DEL LOOP =====
    /**
     * @brief Procesa eventos SDL (quit, teclas, etc.)
     */
    void ProcessInput();

    /**
     * @brief Actualiza la lógica del juego
     * @param deltaTime Tiempo transcurrido en segundos
     */
    void Update(float deltaTime);

    /**
     * @brief Renderiza un frame
     * @param deltaTime Tiempo transcurrido en segundos
     */
    void Render(float deltaTime);
};