#pragma once
#include "../Component.h"
#include "../../external/GLM/include/glm/vec2.hpp"

/**
 * @class ColliderComponent
 * @brief Define el rectángulo de colisión (AABB) de una entidad.
 *
 * Responsabilidades:
 * - Almacenar dimensiones del área de colisión
 * - Proporcionar información para detección de colisiones AABB
 *
 * Geometría:
 * - Se asume que el punto de pivote es la ESQUINA SUPERIOR IZQUIERDA
 * - Posición (de TransformComponent) + Bounds = AABB
 * - Colisión: rectangles se solapan en X e Y
 *
 * Usado por:
 * - CollisionSystem: Detecta solapamiento AABB entre entidades
 * - CollisionResponseSystem: Resuelve colisiones
 * - RenderSystem: Usa para dimensiones de renderizado
 *
 * Nota: Los obstáculos tienen colliders más pequeños que sprites
 *       para permitir espacios navegables entre ellos
 *
 * Ejemplos de tamaños (en píxeles):
 * - Jugador: 94x94
 * - Enemigos: 28x28 a 36x36
 * - Obstáculos: 100x100
 */
class ColliderComponent : public Component
{
public:
    // ===== COLISIÓN =====
    glm::vec2 m_Bounds; ///< Tamaño del AABB en píxeles (ancho, alto)

    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el colisionador
     * @param width Ancho en píxeles
     * @param height Alto en píxeles
     */
    ColliderComponent(float width, float height);
};
