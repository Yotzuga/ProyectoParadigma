#pragma once
#include <string>
#include <memory>
#include <cstdint>

/**
 * @class Event
 * @brief Clase base abstracta para todos los eventos.
 *
 * Responsabilidades:
 * - Base para herencia de eventos específicos
 * - Interfaz uniforme para sistema de eventos
 *
 * Patrón Event-Driven:
 * - Sistemas emiten eventos en World::emit()
 * - Otros sistemas procesan eventos en World::poll()
 * - Desacoplamiento entre sistemas
 *
 * Ejemplo:
 * 1. CollisionSystem emite CollisionEvent
 * 2. DamageSystem poll() procesa CollisionEvent
 * 3. Si es PLAYER_ENEMY: aplica daño
 *
 * Tipos de eventos:
 * - CollisionEvent: Detecta colisiones
 * - DamageEvent: Aplicar daño
 * - SpawnEvent: Crear entidad
 * - SpawnEnemyEvent: Crear enemigo específico
 * - WaveStartEvent: Comienza oleada
 * - WaveEndEvent: Termina oleada
 * - ObstacleSpawnEvent: Crear obstáculo
 */
class Event
{
public:
    /**
     * @brief Destructor virtual para limpieza segura
     */
    virtual ~Event() = default;

    /**
     * @brief Obtiene el tipo de evento como string
     * @return Identificador del tipo (ej: "CollisionEvent")
     */
    virtual std::string getType() const = 0;
};

// ========================================
// TIPOS DE COLISIÓN
// ========================================
/**
 * @enum CollisionType
 * @brief Clasificación de tipos de colisiones detectadas
 */
enum class CollisionType
{
    PLAYER_ENEMY,    ///< Jugador colisiona con enemigo
    ENEMY_ENEMY,     ///< Enemigo colisiona con enemigo
    ENTITY_WORLD,    ///< Entidad colisiona con borde del mundo
    PLAYER_OBSTACLE, ///< Jugador colisiona con obstáculo
    ENEMY_OBSTACLE   ///< Enemigo colisiona con obstáculo
};

// ========================================
// EVENTO: COLISIÓN
// ========================================
/**
 * @class CollisionEvent
 * @brief Notifica de una colisión detectada entre dos entidades.
 *
 * Responsabilidades:
 * - Informar tipo de colisión
 * - Proporcionar IDs de entidades involucradas
 * - Dar dirección de empuje para resolución
 *
 * Procesadores:
 * - CollisionResponseSystem: Aplica push basado en dirección
 * - DamageSystem: Aplica daño si es colisión de daño
 */
class CollisionEvent : public Event
{
public:
    // ===== DATOS DE COLISIÓN =====
    CollisionType collisionType; ///< Tipo de colisión
    uint32_t entityA_Id;         ///< ID de primera entidad (jugador o enemigo)
    uint32_t entityB_Id;         ///< ID de segunda entidad (enemigo, 0 si es mundo)
    float pushDirectionX;        ///< Dirección de empuje X (normalizada -1 a 1)
    float pushDirectionY;        ///< Dirección de empuje Y (normalizada -1 a 1)

    /**
     * @brief Constructor
     * @param type Tipo de colisión
     * @param idA ID de primera entidad
     * @param idB ID de segunda entidad
     * @param pushX Componente X de dirección de empuje
     * @param pushY Componente Y de dirección de empuje
     */
    CollisionEvent(CollisionType type, uint32_t idA, uint32_t idB, float pushX, float pushY);

    std::string getType() const override;
};

// ========================================
// EVENTO: DAÑO
// ========================================
/**
 * @class DamageEvent
 * @brief Notifica que una entidad recibió daño.
 *
 * Procesadores:
 * - DamageSystem: Aplica daño a HealthComponent
 */
class DamageEvent : public Event
{
public:
    uint32_t entityId; ///< ID de la entidad que recibe daño
    int damage;        ///< Cantidad de daño a aplicar

    /**
     * @brief Constructor
     * @param id ID de entidad
     * @param dmg Cantidad de daño
     */
    DamageEvent(uint32_t id, int dmg);

    std::string getType() const override;
};

// ========================================
// EVENTO: SPAWN GENÉRICO
// ========================================
/**
 * @class SpawnEvent
 * @brief Notifica creación de una entidad genérica.
 *
 * Nota: Generalmente se usan eventos específicos como SpawnEnemyEvent
 *       Este es para casos generales o para logging
 */
class SpawnEvent : public Event
{
public:
    std::string entityType; ///< Tipo de entidad a crear
    float x, y;             ///< Posición de spawn

    /**
     * @brief Constructor
     * @param type Tipo de entidad
     * @param posX Posición X
     * @param posY Posición Y
     */
    SpawnEvent(const std::string &type, float posX, float posY);

    std::string getType() const override;
};

// ========================================
// EVENTOS DEL WAVE SYSTEM
// ========================================

/**
 * @class SpawnEnemyEvent
 * @brief Solicita creación de un enemigo específico.
 *
 * Responsabilidades:
 * - Emitida por WaveSystem según timeline
 * - Procesada por SpawnSystem que crea la entidad
 *
 * Parámetros:
 * - enemyTypeName: Tipo (Enemigo1, Enemigo2, etc.)
 * - speedMultiplier: Factor de velocidad (para dificultad progresiva)
 */
class SpawnEnemyEvent : public Event
{
public:
    std::string enemyTypeName; ///< Nombre del tipo de enemigo (e.g., "Enemigo1")
    float speedMultiplier;     ///< Multiplicador de velocidad para dificultad

    /**
     * @brief Constructor
     * @param typeName Tipo de enemigo
     * @param speedMult Multiplicador de velocidad (default 1.0)
     */
    SpawnEnemyEvent(const std::string &typeName, float speedMult = 1.0f);

    std::string getType() const override;
};

/**
 * @class WaveStartEvent
 * @brief Notifica que inicia una nueva oleada.
 *
 * Procesadores:
 * - HUDSystem: Actualiza indicador de oleada
 * - LifetimeSystem: Se prepara para siguiente oleada
 */
class WaveStartEvent : public Event
{
public:
    int waveNumber; ///< Número de oleada (0-indexed)
    int phaseId;    ///< ID de fase de fondo a activar

    /**
     * @brief Constructor
     * @param wave Número de oleada
     * @param phase ID de fase (para cambiar fondo)
     */
    WaveStartEvent(int wave, int phase);

    std::string getType() const override;
};

/**
 * @class WaveEndEvent
 * @brief Notifica que terminó una oleada.
 *
 * Responsabilidades:
 * - Emitida por WaveSystem al finalizar duración
 * - Procesada por LifetimeSystem que elimina enemigos y obstáculos
 * - Inicia transición a siguiente oleada
 */
class WaveEndEvent : public Event
{
public:
    int waveNumber; ///< Número de oleada que terminó

    /**
     * @brief Constructor
     * @param wave Número de oleada terminada
     */
    WaveEndEvent(int wave);

    std::string getType() const override;
};

/**
 * @class ObstacleSpawnEvent
 * @brief Solicita creación de un obstáculo en posición específica.
 *
 * Responsabilidades:
 * - Emitida por WaveSystem al inicio de oleada
 * - Procesada por SpawnSystem que crea la entidad
 * - Todos los obstáculos de una fase se crean de una vez
 */
class ObstacleSpawnEvent : public Event
{
public:
    float x; ///< Posición X del obstáculo
    float y; ///< Posición Y del obstáculo

    /**
     * @brief Constructor
     * @param posX Posición X
     * @param posY Posición Y
     */
    ObstacleSpawnEvent(float posX, float posY);

    std::string getType() const override;
};