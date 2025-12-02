#pragma once
#include "../ISystem.h"
#include "../World.h"

/**
 * @class MovementSystem
 * @brief Aplica velocidad a las entidades en cada frame.
 *
 * Responsabilidades:
 * - Actualizar posiciones basadas en velocidad: pos += vel * dt
 * - Frame-rate independent: el multiplicador dt asegura movimiento consistente
 *
 * Optimizaciones:
 * - Solo itera Jugador + Enemigos (usando caches O(1) de World)
 * - NO itera obstáculos (son estáticos, vel = 0)
 *
 * Ecuación:
 * - new_position = old_position + velocity * deltaTime
 */
class MovementSystem : public ISystem
{
public:
    MovementSystem();

    /**
     * @brief Actualiza posiciones de todas las entidades dinámicas
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;
};
