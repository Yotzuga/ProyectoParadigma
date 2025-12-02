#pragma once
#include "../ISystem.h"
#include <vector>
#include <cstdint>

/**
 * @class LifetimeSystem
 * @brief Sistema SIMPLE que escucha WaveEndEvent y elimina todos los enemigos de la ola.
 *
 * Responsabilidad ÚNICA:
 * - Procesa WaveEndEvent emitido por WaveSystem
 * - Elimina todos los enemigos acivos cuando termina una ola
 * - Proporciona logs de eliminación
 */
class LifetimeSystem : public ISystem
{
public:
    LifetimeSystem() = default;
    ~LifetimeSystem() override = default;

    /**
     * @brief Procesa WaveEndEvent y elimina todos los enemigos.
     * @param world Referencia al mundo que contiene las entidades
     * @param dt Delta time en segundos (no utilizado)
     */
    void update(World &world, float dt) override;
};
