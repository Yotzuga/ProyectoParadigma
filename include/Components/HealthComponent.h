#pragma once
#include "../Component.h"

/**
 * @class HealthComponent
 * @brief Gestiona puntos de vida de una entidad.
 *
 * Responsabilidades:
 * - Almacenar HP actual y máximo
 * - Aplicar daño
 * - Verificar estado de muerte
 *
 * Usado por:
 * - Jugador: Muerte si HP ≤ 0
 * - Enemigos: Eliminación si HP ≤ 0
 * - DamageSystem: Aplica daño y verifica muerte
 * - HUDSystem: Muestra HP del jugador
 *
 * Nota: Los enemigos mueren en 1 golpe (maxHp = 1)
 *       El jugador puede recibir múltiples golpes (maxHp = 10)
 */
class HealthComponent : public Component
{
public:
    // ===== ESTADO DE SALUD =====
    int hp;    ///< Puntos de vida actuales
    int maxHp; ///< Puntos de vida máximos

    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa con HP máximo
     * @param maxHealth Valor de HP máximo inicial (e.g., 10 para jugador, 1 para enemigo)
     */
    HealthComponent(int maxHealth);

    // ===== MECÁNICA DE SALUD =====
    /**
     * @brief Aplica daño a la entidad
     * @param damage Cantidad de daño (restará hp)
     */
    void takeDamage(int damage);

    /**
     * @brief Verifica si la entidad está muerta
     * @return true si hp <= 0
     */
    bool isDead() const;
};
