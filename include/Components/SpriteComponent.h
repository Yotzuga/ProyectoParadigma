#pragma once
#include <string>
#include <iostream>
#include "../Component.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

/**
 * @class SpriteComponent
 * @brief Almacena la textura de renderizado de una entidad.
 *
 * Responsabilidades:
 * - Cargar textura desde archivo (PNG, etc.)
 * - Mantener referencia a SDL_Texture en memoria de GPU
 * - Liberar textura al destruir
 *
 * Características:
 * - Carga con SDL3_image (IMG_LoadTexture)
 * - Integrado con SDL3 renderer
 * - Loguea errores si la carga falla
 *
 * Usado por:
 * - RenderSystem: Obtiene m_Texture para renderizar
 * - Todas las entidades visuales (Jugador, Enemigos, Obstáculos)
 *
 * Ruta de Sprites:
 * - Jugador: assets/sprites/player.png (94x94)
 * - Enemigo1: assets/sprites/Enemy1.png
 * - Enemigo2: assets/sprites/Enemigo2.png
 * - Enemigo3: assets/sprites/Enemigo3.png
 * - Enemigo4: assets/sprites/Enemigo4.png
 * - Obstáculo: assets/sprites/obstaculo.png (100x100)
 *
 * Ciclo de vida:
 * 1. Constructor carga la textura con IMG_LoadTexture
 * 2. Se usa en RenderSystem durante todo el juego
 * 3. Destructor libera con SDL_DestroyTexture
 */
class SpriteComponent : public Component
{
public:
    // ===== RENDERIZADO =====
    SDL_Texture *m_Texture; ///< Textura en memoria de GPU (propiedad del componente)

    // ===== CONSTRUCTOR & DESTRUCTOR =====
    /**
     * @brief Carga una textura desde archivo
     * @param filePath Ruta del archivo (e.g., "assets/sprites/player.png")
     * @param renderer Contexto SDL3 renderer
     *
     * @throws Loguea error si la carga falla, pero no lanza excepción
     *         (m_Texture será nullptr)
     */
    SpriteComponent(const std::string &filePath, SDL_Renderer *renderer);

    /**
     * @brief Libera la textura
     */
    ~SpriteComponent();
};
