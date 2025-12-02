#include "../include/EntityManager.h"
#include "../include/Entity.h"
#include "../include/Components/EnemyComponent.h"
#include "../include/Components/ObstacleComponent.h"
#include "../include/Components/TransformComponent.h"
#include "../include/Components/SpriteComponent.h"
#include "../include/Components/ColliderComponent.h"
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <random>

// ========================================
// RNG SINGLETON
// ========================================
std::mt19937 &EntityManager::GetRng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

// ========================================
// CREAR ENEMIGO
// ========================================
void EntityManager::CreateEnemy(
    World &world,
    SDL_Renderer *renderer,
    const std::vector<ConfigLoader::EnemyTypeConfig> &enemyTypes,
    const std::string &enemyTypeName,
    float speedMultiplier)
{
    // PASO 1: Buscar el tipo de enemigo por nombre
    const ConfigLoader::EnemyTypeConfig *enemyType = nullptr;
    for (const auto &typeConfig : enemyTypes)
    {
        if (typeConfig.name == enemyTypeName)
        {
            enemyType = &typeConfig;
            break;
        }
    }

    if (!enemyType)
    {
        spdlog::error("EntityManager::CreateEnemy - Tipo no encontrado: {}", enemyTypeName);
        return;
    }

    // PASO 2: Crear entidad base
    Entity &enemy = world.createEntity();

    // PASO 3: Agregar EnemyComponent
    float adjustedSpeed = enemyType->baseSpeed * speedMultiplier;
    auto enemyComp = std::make_unique<EnemyComponent>(
        adjustedSpeed,
        enemyType->focusRange,
        enemyType->velocitySmoothing);
    enemy.AddComponent(std::move(enemyComp));

    // PASO 4: Agregar SpriteComponent
    auto sprite = std::make_unique<SpriteComponent>(enemyType->spritePath, renderer);
    enemy.AddComponent(std::move(sprite));

    // PASO 5: Agregar ColliderComponent
    auto collider = std::make_unique<ColliderComponent>(
        enemyType->colliderWidth,
        enemyType->colliderHeight);
    enemy.AddComponent(std::move(collider));

    // PASO 6: Generar posición aleatoria
    std::mt19937 &rng = GetRng();
    std::uniform_real_distribution<float> posXDist(0.0f, std::max(0.0f, 1280.0f - enemyType->colliderWidth));
    std::uniform_real_distribution<float> posYDist(0.0f, std::max(0.0f, 720.0f - enemyType->colliderHeight));
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);

    float px = posXDist(rng);
    float py = posYDist(rng);

    // PASO 7: Generar dirección y velocidad aleatoria
    float angle = angleDist(rng);
    float vx = std::cos(angle) * adjustedSpeed;
    float vy = std::sin(angle) * adjustedSpeed;

    // PASO 8: Agregar TransformComponent
    auto transform = std::make_unique<TransformComponent>(px, py, vx, vy);
    enemy.AddComponent(std::move(transform));
}

// ========================================
// CREAR OBSTÁCULO
// ========================================
void EntityManager::CreateObstacle(
    World &world,
    SDL_Renderer *renderer,
    const ConfigLoader::ObstacleConfig &config,
    float x,
    float y)
{
    // PASO 1: Crear entidad base
    Entity &obstacle = world.createEntity();

    // PASO 2: Agregar ObstacleComponent
    auto obstacleComp = std::make_unique<ObstacleComponent>(true);
    obstacle.AddComponent(std::move(obstacleComp));

    // PASO 3: Agregar SpriteComponent
    auto sprite = std::make_unique<SpriteComponent>(config.spritePath, renderer);
    obstacle.AddComponent(std::move(sprite));

    // PASO 4: Agregar ColliderComponent
    auto collider = std::make_unique<ColliderComponent>(
        config.colliderWidth,
        config.colliderHeight);
    obstacle.AddComponent(std::move(collider));

    // PASO 5: Agregar TransformComponent (SIN velocidad, es estático)
    auto transform = std::make_unique<TransformComponent>(x, y, 0.0f, 0.0f);
    obstacle.AddComponent(std::move(transform));
}
