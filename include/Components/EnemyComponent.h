#pragma once
#include "../Component.h"

/**
 * @class EnemyComponent
 * @brief Define parámetros de IA y comportamiento del enemigo.
 *
 * Responsabilidades:
 * - Almacenar parámetros de IA (velocidad, rango, suavizado)
 * - Permitir variabilidad de enemigos (diferentes tipos)
 * - Proporcionar datos al EnemyAISystem
 *
 * Usado por:
 * - EnemyAISystem: Lee estos parámetros para calcular comportamiento
 * - EntityFactory: Inicializa estos valores desde JSON
 *
 * Tipos de enemigos (definidos en JSON):
 * - Enemigo1: Velocidad baja, rango pequeño
 * - Enemigo2: Velocidad media, rango medio
 * - Enemigo3: Velocidad alta, rango grande
 * - Enemigo4: Velocidad baja, rango medio
 *
 * Parámetro velocitySmoothing controla qué tan rápido el enemigo
 * cambia su vector de velocidad (Lerp smooth vs salto)
 */
class EnemyComponent : public Component
{
public:
    // ===== PARÁMETROS DE IA =====
    float baseSpeed;         ///< Velocidad base del enemigo (píxeles/segundo)
    float focusRange;        ///< Rango de detección/persecución del jugador (píxeles)
    float velocitySmoothing; ///< Factor Lerp para suavizado (0.1-1.0, mayor = cambio más rápido)

    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa un enemigo con parámetros de IA
     * @param speed Velocidad base de movimiento (ej: 200 px/s)
     * @param range Rango de persecución (ej: 400 píxeles)
     * @param smoothing Factor Lerp de suavizado (ej: 0.5)
     */
    EnemyComponent(float speed, float range, float smoothing);

    // ===== DESTRUCTOR =====
    ~EnemyComponent() override = default;
};
