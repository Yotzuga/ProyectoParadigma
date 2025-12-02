#include "../../include/Components/PlayerComponent.h"

PlayerComponent::PlayerComponent()
    : Component(), invulnerabilityTime(0.0f), isInvulnerable(false),
      flashDuration(0.0f), flashInterval(0.1f), isFlashing(false)
{
    m_Type = "PlayerComponent";
}

void PlayerComponent::SetInvulnerable(float duration)
{
    // Activar invulnerabilidad por X segundos
    // En Mega Man, típicamente 3-4 segundos de parpadeo
    invulnerabilityTime = duration;
    isInvulnerable = true;
    flashDuration = duration;
    isFlashing = true;
}

void PlayerComponent::UpdateInvulnerability(float deltaTime)
{
    // Actualizar el contador de invulnerabilidad
    if (isInvulnerable)
    {
        invulnerabilityTime -= deltaTime;

        if (invulnerabilityTime <= 0.0f)
        {
            invulnerabilityTime = 0.0f;
            isInvulnerable = false;
            isFlashing = false;
        }

        // Actualizar contador de parpadeo para animación visual
        if (isFlashing)
        {
            flashDuration -= deltaTime;
            if (flashDuration <= 0.0f)
            {
                isFlashing = false;
            }
        }
    }
}

bool PlayerComponent::IsInvulnerable() const
{
    return isInvulnerable;
}
