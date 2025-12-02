#pragma once
#include "Entity.h"
#include "World.h"
#include "ConfigLoader.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <random>

// ================================
// FACTORY PARA CREAR ENTIDADES
// ================================
// Encapsula la lógica de creación de enemigos y obstáculos
//
// Responsabilidades:
// - CreateEnemy(): Lógica compleja (RNG posición, RNG velocidad)
// - CreateObstacle(): Lógica simple (posición fija, estático)
//
// Ventajas:
// - SpawnSystem solo despacha eventos
// - EntityFactory encapsula creación
// - Reutilizable desde otros sistemas
// - Fácil extender (agregar CreateProjectile(), etc)

class EntityFactory
{
public:
    // ===================
    // CREAR ENEMIGO
    // ===================
    // Lógica COMPLEJA:
    //   1. Buscar tipo de enemigo por nombre
    //   2. Generar posición aleatoria
    //   3. Generar velocidad aleatoria (basada en ángulo aleatorio)
    //   4. Crear entidad con 4 componentes: EnemyComponent, SpriteComponent, ColliderComponent, TransformComponent
    //
    // Parámetros:
    //   - world: Mundo donde crear la entidad
    //   - renderer: Renderer SDL para cargar sprite
    //   - enemyTypes: Vector de tipos de enemigos (Enemy1, Enemy2, etc)
    //   - enemyTypeName: Nombre del tipo a crear ("Enemigo1", "Enemigo2")
    //   - speedMultiplier: Multiplicador de velocidad (defecto 1.0f)
    static void CreateEnemy(
        World &world,
        SDL_Renderer *renderer,
        const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes,
        const std::string &enemyTypeName,
        float speedMultiplier = 1.0f);

    // ===================
    // CREAR OBSTÁCULO
    // ===================
    // Lógica SIMPLE:
    //   1. Crear entidad
    //   2. Agregar componentes estáticos (sin velocidad)
    //   3. Posición exacta (no aleatoria)
    //   4. Sin IA
    //
    // Parámetros:
    //   - world: Mundo donde crear la entidad
    //   - renderer: Renderer SDL para cargar sprite
    //   - config: Configuración del obstáculo (cargada de JSON)
    //   - x, y: Posición exacta del obstáculo
    static void CreateObstacle(
        World &world,
        SDL_Renderer *renderer,
        const ConfigLoader::ObstacleConfig &config,
        float x,
        float y);

private:
    // RNG singleton - se inicializa una sola vez
    // Garantiza diferentes números aleatorios cada vez que se llama
    static std::mt19937 &GetRng();
};
