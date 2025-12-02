#pragma once
#include "../ISystem.h"
#include "../World.h"

/**
 * @class DamageSystem
 * @brief Gestiona daño, invulnerabilidad y muerte.
 *
 * Responsabilidades:
 * - Procesa CollisionEvent tipo PLAYER_ENEMY
 * - Aplica daño al jugador
 * - Activa invulnerabilidad temporal
 * - Verifica muerte del jugador y termina el juego
 *
 * Mecánica:
 * - Jugador recibe 1 daño por colisión con enemigo
 * - Activar invulnerabilidad por X segundos
 * - Enemigos se eliminan al primer golpe
 * - Si jugador muere (HP ≤ 0), termina el juego
 *
 * Parámetros:
 * - playerInvulnerabilityDuration: Segundos de protección tras daño (típicamente 1.0)
 */
class DamageSystem : public ISystem
{
private:
    // ===== PUNTEROS A ESTADO DEL JUEGO =====
    bool *m_GameRunning; ///< Puntero a flag de juego activo (termina si false)
    bool *m_GameOver;    ///< Puntero a flag de Game Over (jugador muere)

    // ===== PARÁMETROS DE DAÑO =====
    float m_PlayerInvulnerabilityDuration; ///< Duración de invulnerabilidad (segundos)

public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de daño
     * @param gameRunning Puntero a flag de juego activo
     * @param gameOver Puntero a flag de Game Over
     * @param invulnerabilityDuration Duración de invulnerabilidad tras daño (ej: 1.0)
     */
    DamageSystem(bool *gameRunning, bool *gameOver, float invulnerabilityDuration);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Procesa daño y verifica condición de derrota
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;
};
