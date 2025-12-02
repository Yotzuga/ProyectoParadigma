#include "../../include/Systems/RenderSystem.h"
#include "../../include/Game.h"
#include "../../include/Entity.h"
#include "../../include/Components/TransformComponent.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Components/ColliderComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <iomanip>
#include <cmath>

RenderSystem::RenderSystem(const GameContext &ctx)
    : m_Renderer(ctx.renderer), m_ElapsedTime(0.0f)
{
}

void RenderSystem::resetTimer()
{
    m_ElapsedTime = 0.0f;
}

float RenderSystem::getElapsedTime() const
{
    return m_ElapsedTime;
}

void RenderSystem::update(World &world, float dt)
{
    if (!m_Renderer)
        return;

    m_ElapsedTime += dt;

    Entity *player = world.GetPlayer();
    if (player)
    {
        auto *transform = player->GetComponent<TransformComponent>();
        auto *sprite = player->GetComponent<SpriteComponent>();
        auto *collider = player->GetComponent<ColliderComponent>();
        auto *playerComp = player->GetComponent<PlayerComponent>();

        if (transform && sprite && collider)
        {
            float width = collider->m_Bounds.x;
            float height = collider->m_Bounds.y;
            SDL_FRect dstRect{transform->m_Position.x, transform->m_Position.y, width, height};

            // Si el jugador está invulnerable, aplicar efecto de parpadeo visual
            if (playerComp && playerComp->isFlashing)
            {
                // Parpadeo: alternar visibilidad cada 0.1 segundos
                int flashCount = static_cast<int>(playerComp->flashDuration / playerComp->flashInterval);
                if (flashCount % 2 == 0)
                {
                    SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
                }
            }
            else
            {
                SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
            }
        }
    }

    // Dibujar enemigos
    const auto &enemies = world.GetEnemies();
    for (const auto &enemy : enemies)
    {
        auto *transform = enemy->GetComponent<TransformComponent>();
        auto *sprite = enemy->GetComponent<SpriteComponent>();
        auto *collider = enemy->GetComponent<ColliderComponent>();

        if (transform && sprite && collider)
        {
            float width = collider->m_Bounds.x;
            float height = collider->m_Bounds.y;
            SDL_FRect dstRect{transform->m_Position.x, transform->m_Position.y, width, height};
            SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
        }
    }

    // Dibujar obstáculos (cuando existan)
    const auto &obstacles = world.GetObstacles();

    for (const auto &obstacle : obstacles)
    {
        auto *transform = obstacle->GetComponent<TransformComponent>();
        auto *sprite = obstacle->GetComponent<SpriteComponent>();
        auto *collider = obstacle->GetComponent<ColliderComponent>();

        if (transform && sprite && collider)
        {
            if (sprite->m_Texture)
            {
                float width = collider->m_Bounds.x;
                float height = collider->m_Bounds.y;
                SDL_FRect dstRect{transform->m_Position.x, transform->m_Position.y, width, height};
                SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
                // Logging removido para evitar spam en cada frame
            }
            else
            {
                spdlog::error("RenderSystem: Obstáculo ID={} no tiene textura cargada", obstacle->m_Id);
            }
        }
        else
        {
            spdlog::error("RenderSystem: Obstáculo ID={} falta componentes: transform={} sprite={} collider={}",
                          obstacle->m_Id, transform != nullptr, sprite != nullptr, collider != nullptr);
        }
    }
}
