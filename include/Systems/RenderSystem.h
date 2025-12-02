#pragma once
#include "../ISystem.h"
#include "../World.h"
#include <SDL3/SDL.h>

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class RenderSystem
 * @brief Renderiza sprites de todas las entidades.
 *
 * Responsabilidades:
 * - Itera entidades con SpriteComponent
 * - Obtiene posición/tamaño de TransformComponent y ColliderComponent
 * - Renderiza texturas en pantalla
 * - Mantiene timer de tiempo transcurrido (para HUD)
 *
 * Renderizado:
 * - Obtiene SDL_Texture de SpriteComponent
 * - Crea SDL_FRect con posición y dimensiones
 * - Renderiza con SDL_RenderTexture
 *
 * Timer:
 * - Acumula deltaTime para tracking de gameplay
 * - Usado por Game para obtener tiempo de Game Over
 */
class RenderSystem : public ISystem
{
private:
    // ===== CONTEXTO SDL =====
    SDL_Renderer *m_Renderer; ///< Contexto de renderizado SDL3

    // ===== TRACKING DE TIEMPO =====
    float m_ElapsedTime; ///< Tiempo acumulado en segundos

public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de renderizado
     * @param ctx Contexto del juego (contiene renderer)
     */
    RenderSystem(const GameContext &ctx);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Renderiza sprites de todas las entidades
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos (acumulado en m_ElapsedTime)
     */
    void update(World &world, float dt) override;

    // ===== TIMER =====
    /**
     * @brief Reinicia el contador de tiempo a 0
     */
    void resetTimer();

    /**
     * @brief Obtiene el tiempo acumulado
     * @return Tiempo en segundos
     */
    float getElapsedTime() const;
};
