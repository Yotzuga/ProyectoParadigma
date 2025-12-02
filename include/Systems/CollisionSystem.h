#pragma once
#include "../ISystem.h"
#include "../World.h"
#include <glm/glm.hpp>

// Declaración forward de GameContext (se define en Game.h)
struct GameContext;

/**
 * @class CollisionSystem
 * @brief Sistema de DETECCIÓN de colisiones.
 *
 * Responsabilidad: Detectar colisiones entre entidades
 * - Verifica intersecciones AABB
 * - Emite CollisionEvent cuando hay colisión
 * - NO aplica respuestas físicas (eso es responsabilidad de CollisionResponseSystem)
 *
 * Tipos de colisiones detectadas:
 * - PLAYER_ENEMY: Jugador toca enemigo
 * - ENEMY_ENEMY: Dos enemigos se tocan
 * - ENTITY_WORLD: Entidad toca borde del mundo
 * - PLAYER_OBSTACLE: Jugador toca obstáculo estático
 * - ENEMY_OBSTACLE: Enemigo toca obstáculo estático
 */
class CollisionSystem : public ISystem
{
public:
    // ===== CONSTRUCTOR =====
    /**
     * @brief Inicializa el sistema de colisiones
     * @param ctx Contexto del juego (contiene dimensiones del mundo)
     */
    CollisionSystem(const GameContext &ctx);

    // ===== INTERFAZ ISystem =====
    /**
     * @brief Detecta todas las colisiones en el mundo
     * @param world Referencia al mundo
     * @param dt Tiempo transcurrido en segundos
     */
    void update(World &world, float dt) override;

private:
    // ===== CONTEXTO =====
    float m_WorldWidth;  ///< Ancho del mundo (1920 píxeles)
    float m_WorldHeight; ///< Alto del mundo (1080 píxeles)

    // ===== MÉTODOS PRIVADOS =====
    /**
     * @brief Comprueba colisión AABB entre dos rectángulos
     * @param pos1 Posición del primer rectángulo
     * @param size1 Tamaño del primer rectángulo
     * @param pos2 Posición del segundo rectángulo
     * @param size2 Tamaño del segundo rectángulo
     * @return true si hay solapamiento en X e Y
     */
    bool checkAABBCollision(const glm::vec2 &pos1, const glm::vec2 &size1,
                            const glm::vec2 &pos2, const glm::vec2 &size2);

    /**
     * @brief Detecta colisiones jugador-enemigo
     * @param world Referencia al mundo
     */
    void checkPlayerEnemyCollisions(World &world);

    /**
     * @brief Detecta colisiones enemigo-enemigo
     * @param world Referencia al mundo
     */
    void checkEnemyEnemyCollisions(World &world);

    /**
     * @brief Detecta colisiones con bordes del mundo
     * @param world Referencia al mundo
     */
    void checkWorldBoundaries(World &world);

    /**
     * @brief Detecta colisiones jugador-obstáculo
     * @param world Referencia al mundo
     */
    void checkPlayerObstacleCollisions(World &world);

    /**
     * @brief Detecta colisiones enemigo-obstáculo
     * @param world Referencia al mundo
     */
    void checkEnemyObstacleCollisions(World &world);
};
