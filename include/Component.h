#pragma once
#include <string>
#include <iostream>

/**
 * @class Component
 * @brief Clase base abstracta para todos los componentes.
 *
 * Responsabilidades:
 * - Base para herencia (destructor virtual)
 * - Identificación de tipo mediante m_Type
 * - Interfaz uniforme para acceso al tipo
 *
 * Patrón:
 * - Cada componente derivado asigna m_Type en su constructor
 * - Entity usa GetComponent<T>() para búsqueda por tipo con RTTI
 * - m_Type es complementario (para debugging/logging)
 *
 * Componentes derivados:
 * - TransformComponent: Posición y velocidad
 * - ColliderComponent: Geometría de colisión (AABB)
 * - SpriteComponent: Renderizado visual
 * - PlayerComponent: Estado del jugador (invulnerabilidad, etc.)
 * - EnemyComponent: Parámetros de IA
 * - HealthComponent: Puntos de vida
 * - ObstacleComponent: Marcador de obstáculo estático
 */
class Component
{
public:
    // ===== IDENTIFICACIÓN =====
    std::string m_Type; ///< Nombre del tipo de componente (para debugging)

    // ===== CONSTRUCTOR & DESTRUCTOR =====
    /**
     * @brief Constructor por defecto
     */
    Component();

    /**
     * @brief Destructor virtual para herencia segura
     */
    virtual ~Component();

    // ===== ACCESO =====
    /**
     * @brief Obtiene el tipo del componente
     * @return Referencia const al string de tipo
     */
    const std::string &getType() const;
};