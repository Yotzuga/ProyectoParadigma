#pragma once
#include "Entity.h"
#include "World.h"
#include "ConfigLoader.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <random>

/**
 * @class EntityManager
 * @brief Utilidad para crear y gestionar entidades complejas.
 *
 * Responsabilidades:
 * - Crear enemigos con configuración desde JSON
 * - Crear obstáculos en posiciones específicas
 * - Generar posiciones y velocidades aleatorias
 * - Ensamblar componentes para entidades completas
 *
 * Nota: NO es un Factory Pattern clásico.
 *       Es un conjunto de funciones helper para creación de entidades.
 *
 * Uso:
 *   EntityManager::CreateEnemy(world, renderer, types, "Enemigo1", 1.5f);
 *   EntityManager::CreateObstacle(world, renderer, config, x, y);
 */
class EntityManager
{
public:
    // ===================
    // CREAR ENEMIGO
    // ===================
    /**
     * @brief Crea un enemigo completamente configurado
     *
     * Lógica COMPLEJA:
     *   1. Buscar tipo de enemigo por nombre
     *   2. Generar posición aleatoria
     *   3. Generar velocidad aleatoria (basada en ángulo aleatorio)
     *   4. Crear entidad con 4 componentes:
     *      - EnemyComponent, SpriteComponent, ColliderComponent, TransformComponent
     *
     * @param world Mundo donde crear la entidad
     * @param renderer Renderer SDL para sprites
     * @param enemyTypes Vector de configuraciones de enemigos
     * @param enemyTypeName Nombre del tipo ("Enemigo1", etc.)
     * @param speedMultiplier Multiplicador de velocidad (default 1.0)
     */
    static void CreateEnemy(
        World &world,
        SDL_Renderer *renderer,
        const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes,
        const std::string &enemyTypeName,
        float speedMultiplier = 1.0f);

    // ===================
    // CREAR OBSTÁCULO
    // ===================
    /**
     * @brief Crea un obstáculo estático en posición específica
     *
     * Lógica SIMPLE:
     *   1. Crear entidad
     *   2. Agregar componentes estáticos (sin velocidad)
     *   3. Posición exacta (no aleatoria)
     *   4. Sin IA
     *
     * @param world Mundo donde crear la entidad
     * @param renderer Renderer SDL para sprites
     * @param config Configuración del obstáculo
     * @param x Posición X
     * @param y Posición Y
     */
    static void CreateObstacle(
        World &world,
        SDL_Renderer *renderer,
        const ConfigLoader::ObstacleConfig &config,
        float x,
        float y);

    // ===================
    // EXTENSIÓN FUTURA
    // ===================
    // Puedes agregar más funciones helper aquí:
    // static void CreateProjectile(...);
    // static void CreatePowerUp(...);
    // static void CreateParticle(...);
    // static Entity* FindNearestEnemy(World &world, float x, float y);
    // static void ClearAllDead(World &world);
    // static int GetEnemyCount(World &world);

private:
    /**
     * @brief Obtiene generador de números aleatorios (singleton)
     * @return Referencia a std::mt19937
     */
    static std::mt19937 &GetRng();
};
