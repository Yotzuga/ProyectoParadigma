#include "../../include/Systems/CollisionSystem.h"
#include "../../include/Game.h"
#include "../../include/Entity.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/EnemyComponent.h"
#include "../../include/Components/TransformComponent.h"
#include "../../include/Components/ColliderComponent.h"
#include "../../include/Event.h"
#include <spdlog/spdlog.h>

CollisionSystem::CollisionSystem(const GameContext &ctx)
    : m_WorldWidth(ctx.worldWidth), m_WorldHeight(ctx.worldHeight)
{
    // Constructor simple, sin logging
}

bool CollisionSystem::checkAABBCollision(const glm::vec2 &pos1, const glm::vec2 &size1,
                                         const glm::vec2 &pos2, const glm::vec2 &size2)
{
    const float r1Left = pos1.x;
    const float r1Right = pos1.x + size1.x;
    const float r1Top = pos1.y;
    const float r1Bottom = pos1.y + size1.y;

    const float r2Left = pos2.x;
    const float r2Right = pos2.x + size2.x;
    const float r2Top = pos2.y;
    const float r2Bottom = pos2.y + size2.y;

    return !(r1Left > r2Right || r1Right < r2Left || r1Bottom < r2Top || r1Top > r2Bottom);
}

void CollisionSystem::checkPlayerEnemyCollisions(World &world)
{
    Entity *player = world.GetPlayer();
    if (!player)
        return;

    auto *playerTransform = player->GetComponent<TransformComponent>();
    auto *playerCollider = player->GetComponent<ColliderComponent>();

    if (!playerTransform || !playerCollider)
        return;

    // Iterar solo enemigos
    const auto &enemies = world.GetEnemies();
    for (Entity *enemy : enemies)
    {
        auto *enemyTransform = enemy->GetComponent<TransformComponent>();
        auto *enemyCollider = enemy->GetComponent<ColliderComponent>();

        if (!enemyTransform || !enemyCollider)
            continue;

        if (!checkAABBCollision(playerTransform->m_Position, playerCollider->m_Bounds,
                                enemyTransform->m_Position, enemyCollider->m_Bounds))
            continue;

        glm::vec2 pushDirection = glm::normalize(playerTransform->m_Position - enemyTransform->m_Position);

        world.emit(std::make_unique<CollisionEvent>(
            CollisionType::PLAYER_ENEMY,
            player->m_Id,
            enemy->m_Id,
            pushDirection.x,
            pushDirection.y));
    }
}

void CollisionSystem::checkEnemyEnemyCollisions(World &world)
{
    const auto &enemies = world.GetEnemies();

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        Entity *entityA = enemies[i];

        auto *transformA = entityA->GetComponent<TransformComponent>();
        auto *colliderA = entityA->GetComponent<ColliderComponent>();

        if (!transformA || !colliderA)
            continue;

        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            Entity *entityB = enemies[j];

            auto *transformB = entityB->GetComponent<TransformComponent>();
            auto *colliderB = entityB->GetComponent<ColliderComponent>();

            if (!transformB || !colliderB)
                continue;

            if (!checkAABBCollision(transformA->m_Position, colliderA->m_Bounds,
                                    transformB->m_Position, colliderB->m_Bounds))
                continue;

            glm::vec2 centerA = transformA->m_Position + (colliderA->m_Bounds * 0.5f);
            glm::vec2 centerB = transformB->m_Position + (colliderB->m_Bounds * 0.5f);
            glm::vec2 pushDirection = glm::normalize(centerA - centerB);

            world.emit(std::make_unique<CollisionEvent>(
                CollisionType::ENEMY_ENEMY,
                entityA->m_Id,
                entityB->m_Id,
                pushDirection.x,
                pushDirection.y));
        }
    }
}

void CollisionSystem::checkWorldBoundaries(World &world)
{
    Entity *player = world.GetPlayer();
    if (player)
    {
        auto *t = player->GetComponent<TransformComponent>();
        auto *c = player->GetComponent<ColliderComponent>();
        if (t && c)
        {
            bool collided = false;
            glm::vec2 pushDirection(0.0f, 0.0f);

            if (t->m_Position.x < 0.f)
            {
                pushDirection.x = 1.0f;
                collided = true;
            }
            else if (t->m_Position.x + c->m_Bounds.x > m_WorldWidth)
            {
                pushDirection.x = -1.0f;
                collided = true;
            }

            if (t->m_Position.y < 0.f)
            {
                pushDirection.y = 1.0f;
                collided = true;
            }
            else if (t->m_Position.y + c->m_Bounds.y > m_WorldHeight)
            {
                pushDirection.y = -1.0f;
                collided = true;
            }

            if (collided)
            {
                world.emit(std::make_unique<CollisionEvent>(
                    CollisionType::ENTITY_WORLD,
                    player->m_Id,
                    0,
                    pushDirection.x,
                    pushDirection.y));
            }
        }
    }

    // Verificar enemigos
    const auto &enemies = world.GetEnemies();
    for (Entity *e : enemies)
    {
        auto *t = e->GetComponent<TransformComponent>();
        auto *c = e->GetComponent<ColliderComponent>();

        if (!t || !c)
            continue;

        bool collided = false;
        glm::vec2 pushDirection(0.0f, 0.0f);

        if (t->m_Position.x < 0.f)
        {
            pushDirection.x = 1.0f;
            collided = true;
        }
        else if (t->m_Position.x + c->m_Bounds.x > m_WorldWidth)
        {
            pushDirection.x = -1.0f;
            collided = true;
        }

        if (t->m_Position.y < 0.f)
        {
            pushDirection.y = 1.0f;
            collided = true;
        }
        else if (t->m_Position.y + c->m_Bounds.y > m_WorldHeight)
        {
            pushDirection.y = -1.0f;
            collided = true;
        }

        if (collided)
        {
            world.emit(std::make_unique<CollisionEvent>(
                CollisionType::ENTITY_WORLD,
                e->m_Id,
                0,
                pushDirection.x,
                pushDirection.y));
        }
    }
}

void CollisionSystem::checkPlayerObstacleCollisions(World &world)
{
    Entity *player = world.GetPlayer();
    if (!player)
        return;

    auto *playerTransform = player->GetComponent<TransformComponent>();
    auto *playerCollider = player->GetComponent<ColliderComponent>();

    if (!playerTransform || !playerCollider)
        return;

    // Iterar solo obstáculos
    const auto &obstacles = world.GetObstacles();
    for (Entity *obstacle : obstacles)
    {
        auto *obstacleTransform = obstacle->GetComponent<TransformComponent>();
        auto *obstacleCollider = obstacle->GetComponent<ColliderComponent>();

        if (!obstacleTransform || !obstacleCollider)
            continue;

        if (!checkAABBCollision(playerTransform->m_Position, playerCollider->m_Bounds,
                                obstacleTransform->m_Position, obstacleCollider->m_Bounds))
            continue;

        glm::vec2 pushDirection = glm::normalize(playerTransform->m_Position - obstacleTransform->m_Position);

        world.emit(std::make_unique<CollisionEvent>(
            CollisionType::PLAYER_OBSTACLE,
            player->m_Id,
            obstacle->m_Id,
            pushDirection.x,
            pushDirection.y));
    }
}

void CollisionSystem::checkEnemyObstacleCollisions(World &world)
{
    const auto &enemies = world.GetEnemies();
    const auto &obstacles = world.GetObstacles();

    for (Entity *enemy : enemies)
    {
        auto *enemyTransform = enemy->GetComponent<TransformComponent>();
        auto *enemyCollider = enemy->GetComponent<ColliderComponent>();

        if (!enemyTransform || !enemyCollider)
            continue;

        for (Entity *obstacle : obstacles)
        {
            auto *obstacleTransform = obstacle->GetComponent<TransformComponent>();
            auto *obstacleCollider = obstacle->GetComponent<ColliderComponent>();

            if (!obstacleTransform || !obstacleCollider)
                continue;

            if (!checkAABBCollision(enemyTransform->m_Position, enemyCollider->m_Bounds,
                                    obstacleTransform->m_Position, obstacleCollider->m_Bounds))
                continue;

            glm::vec2 pushDirection = glm::normalize(enemyTransform->m_Position - obstacleTransform->m_Position);

            world.emit(std::make_unique<CollisionEvent>(
                CollisionType::ENEMY_OBSTACLE,
                enemy->m_Id,
                obstacle->m_Id,
                pushDirection.x,
                pushDirection.y));
        }
    }
}

void CollisionSystem::update(World &world, float dt)
{
    checkPlayerEnemyCollisions(world);
    checkEnemyEnemyCollisions(world);
    checkWorldBoundaries(world);
    checkPlayerObstacleCollisions(world);
    checkEnemyObstacleCollisions(world);
}
