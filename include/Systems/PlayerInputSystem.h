#pragma once
#include "../ISystem.h"
#include "../World.h"
#include <SDL3/SDL.h>

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class PlayerInputSystem
 * @brief Procesa input del teclado y actualiza velocidad del jugador.
 *
 * Responsabilidades:
 * - Leer estado del teclado (W/A/S/D)
 * - Calcular vector de velocidad deseado
 * - Asignar velocidad al jugador
 *
 * Input Mapping:
 * - W: Arriba (velocity.y = -playerSpeed)
 * - S: Abajo (velocity.y = +playerSpeed)
 * - A: Izquierda (velocity.x = -playerSpeed)
 * - D: Derecha (velocity.x = +playerSpeed)
 *
 * Frame-rate Independence:
 * - Se calcula velocidad en px/s, la multiplicación por dt ocurre en MovementSystem
 *
 * Nota: m_WorldWidth/m_WorldHeight están disponibles para futuras mejoras (boundary checking)
 */
class PlayerInputSystem : public ISystem
{
private:
    // ===== PARÁMETROS =====
    float m_PlayerSpeed;    ///< Velocidad del jugador en píxeles/segundo
    float m_WorldWidth;     ///< Ancho del mundo (para posibles restricciones)
    float m_WorldHeight;    ///< Alto del mundo (para posibles restricciones)
    const bool *m_KeyState; ///< Puntero al estado del teclado SDL

    // ===== MÉTODOS PRIVADOS =====
    /**
     * @brief Actualiza m_KeyState con el estado actual del teclado SDL
     */
    void updateKeyState();

public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de input
     * @param ctx Contexto del juego (renderer, dimensions)
     * @param playerSpeed Velocidad del jugador en px/s
     */
    PlayerInputSystem(const GameContext &ctx, float playerSpeed);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Procesa input y asigna velocidad al jugador
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos (no usado en este sistema)
     */
    void update(World &world, float dt) override;
};
