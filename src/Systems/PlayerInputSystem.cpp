#include "../../include/Systems/PlayerInputSystem.h"
#include "../../include/Game.h"
#include "../../include/Entity.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformComponent.h"
#include "../../include/Components/ColliderComponent.h"
#include <algorithm>

PlayerInputSystem::PlayerInputSystem(const GameContext &ctx, float playerSpeed)
    : m_PlayerSpeed(playerSpeed), m_WorldWidth(ctx.worldWidth), m_WorldHeight(ctx.worldHeight), m_KeyState(nullptr)
{
}

void PlayerInputSystem::updateKeyState()
{
    m_KeyState = SDL_GetKeyboardState(nullptr);
}

void PlayerInputSystem::update(World &world, float dt)
{
    // Actualizar estado del teclado
    updateKeyState();

    if (!m_KeyState)
        return;

    // ✅ Obtener jugador con O(1) en lugar de iterar todas las entidades
    Entity *entity = world.GetPlayer();
    if (!entity)
        return;

    auto *player = entity->GetComponent<PlayerComponent>();
    if (!player)
        return;

    auto *transform = entity->GetComponent<TransformComponent>();
    auto *collider = entity->GetComponent<ColliderComponent>();

    if (!transform)
        return;

    // Calcular velocidad basada en input WASD
    // Frame-rate independent: velocidad se multiplica por dt en MovementSystem
    glm::vec2 velocity(0.0f, 0.0f);

    if (m_KeyState[SDL_SCANCODE_W])
    {
        velocity.y -= m_PlayerSpeed;
    }
    if (m_KeyState[SDL_SCANCODE_S])
    {
        velocity.y += m_PlayerSpeed;
    }
    if (m_KeyState[SDL_SCANCODE_A])
    {
        velocity.x -= m_PlayerSpeed;
    }
    if (m_KeyState[SDL_SCANCODE_D])
    {
        velocity.x += m_PlayerSpeed;
    }

    // La velocidad se multiplica por dt en MovementSystem
    // Esto asegura que el movimiento sea consistente independientemente del frame rate
    transform->m_Velocity = velocity;
}
