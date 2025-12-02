#pragma once
#include "../Component.h"
#include "../../external/GLM/include/glm/vec2.hpp"

/**
 * @class TransformComponent
 * @brief Define posición y velocidad de una entidad en el mundo.
 *
 * Responsabilidades:
 * - Almacenar posición (x, y) en píxeles
 * - Almacenar velocidad (vx, vy) en píxeles/segundo
 *
 * Usado por:
 * - MovementSystem: Aplica velocidad a posición
 * - RenderSystem: Obtiene posición para renderizar
 * - CollisionSystem: Obtiene posición para detectar colisiones
 * - EnemyAISystem: Modifica velocidad según IA
 * - PlayerInputSystem: Modifica velocidad según input
 *
 * Nota: Las actualizaciones de velocidad ocurren en otros sistemas
 *       TransformComponent solo almacena los valores
 */
class TransformComponent : public Component
{
public:
    // ===== TRANSFORMACIÓN =====
    glm::vec2 m_Position; ///< Posición actual en el mundo (píxeles)
    glm::vec2 m_Velocity; ///< Velocidad actual (píxeles/segundo)

    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa la transformación
     * @param x Posición inicial X
     * @param y Posición inicial Y
     * @param vx Velocidad inicial X
     * @param vy Velocidad inicial Y
     */
    TransformComponent(float x, float y, float vx, float vy);
};
