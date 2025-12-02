#pragma once
#include "../Component.h"

/**
 * @class PlayerComponent
 * @brief Define el estado y comportamiento específico del jugador.
 *
 * Responsabilidades:
 * - Rastrear invulnerabilidad temporal
 * - Rastrear estado de parpadeo visual
 * - Proporcionar métodos para activar/desactivar protección
 *
 * Mecánica:
 * - Cuando el jugador recibe daño, se activa invulnerabilidad
 * - Durante invulnerabilidad, no puede recibir más daño
 * - Se visualiza con parpadeo para feedback al usuario
 *
 * Usado por:
 * - DamageSystem: Activa invulnerabilidad al recibir daño
 * - RenderSystem: Comprueba parpadeo para renderizar/no renderizar
 *
 * Nota: El jugador también tiene TransformComponent, HealthComponent, etc.
 */
class PlayerComponent : public Component
{
public:
    // ===== INVULNERABILIDAD =====
    float invulnerabilityTime; ///< Tiempo restante de invulnerabilidad (segundos)
    bool isInvulnerable;       ///< ¿Está actualmente inmune al daño?

    // ===== PARPADEO VISUAL =====
    float flashDuration; ///< Duración total del efecto de parpadeo (segundos)
    float flashInterval; ///< Intervalo entre toggles de visibilidad (segundos)
    bool isFlashing;     ///< ¿Está actualmente parpadeando?

    // ===== CONSTRUCTOR =====
    PlayerComponent();

    // ===== CONTROL DE INVULNERABILIDAD =====
    /**
     * @brief Activa invulnerabilidad temporal
     * @param duration Duración en segundos
     */
    void SetInvulnerable(float duration);

    /**
     * @brief Actualiza el contador de invulnerabilidad
     * @param deltaTime Tiempo transcurrido en segundos
     */
    void UpdateInvulnerability(float deltaTime);

    /**
     * @brief Verifica si está actualmente protegido
     * @return true si está invulnerable
     */
    bool IsInvulnerable() const;
};
