#pragma once

class World; // Forward declaration

/**
 * @struct ISystem
 * @brief Interfaz base para todos los sistemas ECS.
 *
 * Responsabilidades:
 * - Definir la interfaz uniforme de actualización
 * - Permitir herencia para especialización de sistemas
 *
 * Patrón de Sistemas:
 * - Cada sistema especializado (PlayerInputSystem, MovementSystem, etc.)
 * - Implementa update(World &world, float dt)
 * - Se invoca en orden específico desde Game::Update()
 *
 * Orden de ejecución típico:
 * 1. WaveManagerSystem (orquestación)
 * 2. PlayerInputSystem (input)
 * 3. EnemyAISystem (comportamiento)
 * 4. MovementSystem (física)
 * 5. CollisionSystem (detección)
 * 6. CollisionResponseSystem (resolución)
 * 7. DamageSystem (lógica de daño)
 * 8. RenderSystem (renderizado)
 * 9. HUDSystem (interfaz)
 *
 * Nota: El orden es crítico para evitar comportamientos incorrectos
 *       (ej: debe haber movimiento antes de colisiones)
 */
struct ISystem
{
    /**
     * @brief Actualiza la lógica del sistema
     * @param world Referencia al mundo (contiene todas las entidades)
     * @param dt Tiempo transcurrido en segundos desde último frame
     */
    virtual void update(World &world, float dt) = 0;

    /**
     * @brief Destructor virtual para limpieza segura
     */
    virtual ~ISystem() = default;
};