#pragma once
#include "../ISystem.h"
#include "../World.h"

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class CollisionResponseSystem
 * @brief Sistema de RESOLUCIÓN de colisiones.
 *
 * Responsabilidades:
 * - Procesa CollisionEvent emitidos por CollisionSystem
 * - Aplica respuestas físicas: push/bounce para evitar solapamiento
 * - Confina entidades dentro de los límites del mundo
 *
 * Respuestas por tipo de colisión:
 * - PLAYER_ENEMY: Ambas entidades se empujan (playerPush, enemyPush)
 * - ENEMY_ENEMY: Se empujan mutuamente
 * - ENTITY_WORLD: Se desplazan hacia dentro del mundo
 * - PLAYER_OBSTACLE: Jugador se empuja fuera del obstáculo
 * - ENEMY_OBSTACLE: Enemigo se empuja fuera del obstáculo
 *
 * Parámetros:
 * - playerPushStrength: Fuerza con que el jugador empuja a otros
 * - enemyPushStrength: Fuerza con que los enemigos se empujan entre sí
 */
class CollisionResponseSystem : public ISystem
{
public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de respuesta de colisiones
     * @param ctx Contexto del juego (dimensions del mundo)
     * @param playerPush Fuerza de empuje del jugador (típicamente 10)
     * @param enemyPush Fuerza de empuje entre enemigos (típicamente 10)
     */
    CollisionResponseSystem(const GameContext &ctx, float playerPush, float enemyPush);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Procesa eventos de colisión y aplica respuestas
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;

private:
    // ===== PARÁMETROS FÍSICOS =====
    float m_PlayerPushStrength; ///< Fuerza de empuje del jugador
    float m_EnemyPushStrength;  ///< Fuerza de empuje entre enemigos
    float m_WorldWidth;         ///< Ancho del mundo (para confinar)
    float m_WorldHeight;        ///< Alto del mundo (para confinar)
};
