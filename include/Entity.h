#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include "Component.h"
#include <vector>
#include <memory>

/**
 * @class Entity
 * @brief Contenedor base para componentes en la arquitectura ECS.
 *
 * Una entidad es un agregado de componentes que definen su comportamiento.
 * Puede ser: Jugador, Enemigo, Obstáculo, etc.
 *
 * Características:
 * - Identificador único (uint32_t m_Id)
 * - Nombre simbólico (para debugging)
 * - Contenedor dinámico de componentes
 * - Búsqueda de componentes por tipo con templates
 */
class Entity
{
private:
    // ===== COMPONENTES =====
    std::vector<std::unique_ptr<Component>> m_Components; ///< Lista de componentes asociados

public:
    // ===== IDENTIDAD =====
    uint32_t m_Id;      ///< Identificador único (asignado por World)
    std::string m_Name; ///< Nombre simbólico (para debugging)

    // ===== CONSTRUCTORES & DESTRUCTORES =====
    Entity();
    Entity(uint32_t id, const std::string &name);
    ~Entity();

    // ===== GESTIÓN DE COMPONENTES =====
    /**
     * @brief Agrega un componente a la entidad
     * @param component Componente a agregar (transferencia de propiedad)
     */
    void AddComponent(std::unique_ptr<Component> component);

    /**
     * @brief Obtiene un componente por tipo (búsqueda O(n))
     * @tparam T Tipo del componente a buscar
     * @return Puntero al componente o nullptr si no existe
     */
    template <typename T>
    T *GetComponent() const
    {
        for (const auto &comp : m_Components)
        {
            T *casted = dynamic_cast<T *>(comp.get());
            if (casted)
                return casted;
        }
        return nullptr;
    }

    // ===== ACCESSORS =====
    uint32_t getId() const;
    std::string getName() const;
};
