#include "../../include/Components/EnemyComponent.h"

EnemyComponent::EnemyComponent(float speed, float range, float smoothing)
    : Component(), baseSpeed(speed), focusRange(range), velocitySmoothing(smoothing)
{
    m_Type = "EnemyComponent";
}
