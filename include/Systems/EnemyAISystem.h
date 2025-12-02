#pragma once
#include "../ISystem.h"
#include "../World.h"

/**
 * @class EnemyAISystem
 * @brief Sistema de IA de enemigos.
 *
 * Responsabilidades:
 * - Calcular velocidad de cada enemigo basada en posición del jugador
 * - Implementar persecución con rango de visión
 * - Suavizar cambios de velocidad (Lerp) para movimiento natural
 *
 * Algoritmo de IA:
 * 1. Para cada enemigo:
 *    a. Calcular distancia al jugador
 *    b. Si distancia < focusRange: perseguir
 *    c. Si distancia >= focusRange: deambular o quieto
 * 2. Suavizar velocidad usando Lerp: vel = Lerp(vel_actual, vel_deseada, smoothing)
 * 3. Asignar velocidad a TransformComponent
 *
 * Parámetros por tipo de enemigo:
 * - baseSpeed: Velocidad máxima (ej: 200-400 px/s)
 * - focusRange: Rango de persecución (ej: 400-500 px)
 * - velocitySmoothing: Factor Lerp (0.1-1.0, mayor = cambio más rápido)
 *
 * Comportamiento:
 * - Persecución: Se mueve hacia el jugador dentro del rango
 * - Fuera de rango: Velocidad se suaviza a cero (deambula lentamente)
 */
class EnemyAISystem : public ISystem
{
public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de IA
     */
    EnemyAISystem();

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Actualiza velocidad de todos los enemigos
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos (usado para Lerp)
     */
    void update(World &world, float dt) override;
};
