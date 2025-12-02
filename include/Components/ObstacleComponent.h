#pragma once
#include "../Component.h"

/**
 * @class ObstacleComponent
 * @brief Identifica una entidad como obstáculo estático del mundo.
 *
 * Responsabilidades:
 * - Marcar entidad como obstáculo (para clasificación en World)
 * - Indicar que NO debe ser actualizado por MovementSystem
 * - Permitir que el CollisionSystem detecte colisiones con obstáculos
 *
 * Características:
 * - Componente PASIVO: sin lógica de actualización
 * - Siempre isStatic = true (no se mueven)
 * - No tiene velocidad en TransformComponent
 *
 * Componentes típicos de un obstáculo:
 * - TransformComponent (solo posición, vel = 0)
 * - ColliderComponent (define AABB para colisiones)
 * - SpriteComponent (renderizado visual)
 * - ObstacleComponent (clasificación)
 *
 * Usado por:
 * - World: Clasificación en caché de obstáculos
 * - CollisionSystem: Detección de colisiones
 * - MovementSystem: Excluye de iteración (vel = 0)
 */
class ObstacleComponent : public Component
{
public:
    // ===== PROPIEDAD =====
    bool isStatic; ///< Siempre true (obstáculos nunca se mueven)

    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa componente de obstáculo
     * @param static_flag Debe ser true (para validación lógica)
     */
    ObstacleComponent(bool static_flag);

    // ===== DESTRUCTOR =====
    ~ObstacleComponent() override = default;
};
