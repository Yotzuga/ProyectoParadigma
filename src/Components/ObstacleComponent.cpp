#include "../../include/Components/ObstacleComponent.h"

ObstacleComponent::ObstacleComponent(bool static_flag)
    : Component(), isStatic(static_flag)
{
    m_Type = "ObstacleComponent";
}
