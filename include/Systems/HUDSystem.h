#pragma once
#include "../ISystem.h"
#include "../World.h"
#include <SDL3/SDL.h>
#include <string>

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class HUDSystem
 * @brief Sistema de HUD (Heads-Up Display).
 *
 * Responsabilidades:
 * - Renderizar información en tiempo real sobre la pantalla
 * - Mostrar tiempo transcurrido (desde RenderSystem)
 * - Mostrar HP del jugador (desde HealthComponent)
 * - Mostrar indicadores visuales (barras de vida, etc.)
 *
 * Información mostrada:
 * - Tiempo jugado en segundos
 * - Puntos de vida del jugador (ej: "HP: 8/10")
 * - Estado de oleada actual (futuro)
 *
 * Renderizado:
 * - Utiliza SDL3 para dibujar texto y rectángulos
 * - Colores: Blanco para texto, Rojo para barras de daño
 * - Posicionado en esquinas de la pantalla
 *
 * Nota: El renderizado de HUD es independiente de la capa de sprites
 *       Se renderiza DESPUÉS de los sprites para aparecer encima
 */
class HUDSystem : public ISystem
{
private:
    // ===== CONTEXTO SDL =====
    SDL_Renderer *m_Renderer; ///< Contexto de renderizado SDL3
    float m_WorldWidth;       ///< Ancho para posicionar HUD
    float m_WorldHeight;      ///< Alto para posicionar HUD

    // ===== MÉTODOS PRIVADOS =====
    /**
     * @brief Renderiza texto en pantalla (interfaz futura)
     * @param text Texto a renderizar
     * @param x Posición X
     * @param y Posición Y
     */
    void renderText(const std::string &text, int x, int y);

    /**
     * @brief Renderiza un rectángulo de color
     * @param x Posición X
     * @param y Posición Y
     * @param width Ancho
     * @param height Alto
     * @param r Valor R (0-255)
     * @param g Valor G (0-255)
     * @param b Valor B (0-255)
     * @param a Valor Alpha (0-255), default 255
     */
    void renderRectangle(int x, int y, int width, int height, int r, int g, int b, int a = 255);

public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de HUD
     * @param ctx Contexto del juego (renderer y dimensions)
     */
    HUDSystem(const GameContext &ctx);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Renderiza HUD en pantalla
     * @param world Referencia al mundo (para obtener jugador)
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;
};
