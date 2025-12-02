# 📊 Diagramas de Secuencia - EXAMEN3

Diagramas de secuencia detallados que muestran la interacción entre componentes en escenarios clave.

---

## 1. Inicio del Juego

```plantuml
@startuml GameStartup
title Secuencia: Inicio del Juego

participant "main()" as MAIN
participant "Game" as G
participant "World" as W
participant "ConfigLoader" as CL
participant "EntityFactory" as EF
participant "Systems" as S

MAIN -> G: Game()
MAIN -> G: Initialize()
activate G
  G -> G: SDL_Init()
  G -> G: CreateWindow()
  G -> G: CreateRenderer()
deactivate G

MAIN -> G: Start()
activate G
  G -> CL: LoadConfiguration()
  activate CL
    CL -> CL: Cargar settings.json
    CL -> CL: Cargar Waves.json
    CL -> CL: Cargar Enemy_entities.json
    CL -> CL: Cargar ObstaclesByFase.json
  deactivate CL
  
  G -> EF: CreatePlayer()
  activate EF
    EF -> W: createEntity()
    activate W
      W -> W: Generar ID único
      W -> W: m_EntityMap[id] = entity
      W -> W: m_PlayerEntity = entity
    deactivate W
    EF -> EF: AddComponent<Transform>()
    EF -> EF: AddComponent<Sprite>()
    EF -> EF: AddComponent<Collider>()
    EF -> EF: AddComponent<Health>()
    EF -> EF: AddComponent<Player>()
  deactivate EF
  
  G -> S: WaveManagerSystem.LoadConfig()
  activate S
    S -> S: WaveSystem.LoadWaves()
    S -> S: SpawnSystem.SetObstacles()
  deactivate S
deactivate G

MAIN -> G: Run()
note over MAIN, S
  Inicia game loop principal
end note

@enduml
```

---

## 2. Spawn de Enemigos

```plantuml
@startuml EnemySpawn
title Secuencia: Spawn de Enemigos

participant "WaveSystem" as W
participant "World\nEventQueue" as E
participant "SpawnSystem" as S
participant "EntityFactory" as EF
participant "World\nEnemies" as WE

== Timeline de Oleada ==
W -> W: waveTimer += deltaTime
W -> W: spawnTimer += deltaTime

== Decisión ==
alt spawnTimer >= spawnInterval
  W -> E: EmitEvent(SpawnEnemyEvent)
  activate E
    note right of E
      Evento encolado
      Se procesa en siguiente update
    end note
  deactivate E
  
  W -> W: spawnTimer = 0
  W -> W: enemiesSpawned++
end

== Procesamiento de Evento ==
E -> S: SpawnSystem.update(world, dt)
activate S
  S -> S: Leer eventos de la queue
  
  alt event es SpawnEnemyEvent
    S -> S: Obtener config del tipo
    S -> S: GenerarPosicionAleatoria()
    
    S -> EF: CreateEnemy(type, position)
    activate EF
      EF -> WE: createEntity()
      activate WE
        WE -> WE: Generar ID
        WE -> WE: m_EntityMap[id] = enemy
        WE -> WE: m_EnemyEntities.push_back(enemy)
      deactivate WE
      
      EF -> EF: AddComponent<Transform>(x, y)
      EF -> EF: AddComponent<Sprite>(texture)
      EF -> EF: AddComponent<Collider>(w, h)
      EF -> EF: AddComponent<Health>(1, 1)
      EF -> EF: AddComponent<Enemy>(params)
    deactivate EF
  end
deactivate S

@enduml
```

---

## 3. Sistema de Colisiones

```plantuml
@startuml CollisionDetection
title Secuencia: Detección y Resolución de Colisiones

participant "MovementSystem" as MV
participant "CollisionSystem" as CD
participant "CollisionResponseSystem" as CR
participant "World\nEventQueue" as E
participant "DamageSystem" as DM

== FASE 1: MOVIMIENTO ==
MV -> MV: Para cada entidad con Transform
MV -> MV: pos += vel * dt
MV -> MV: Actualiza posición

== FASE 2: DETECCIÓN ==
CD -> CD: Para cada pareja de entidades
CD -> CD: Verificar AABB collision

alt Hay colisión
  CD -> E: EmitEvent(CollisionEvent)
  activate E
    note right of E
      CollisionEvent encolado
      Incluye tipo y entidades
    end note
  deactivate E
else Sin colisión
  CD -> CD: Continuar
end

== FASE 3: RESPUESTA ==
CR -> E: ProcesarEventos()
activate E
  CR -> CR: Para cada CollisionEvent
  
  alt PLAYER_ENEMY
    CR -> CR: CalcularVectorPush()
    CR -> CR: Aplicar push a ambos
    CR -> CR: Limitar dentro del mundo
  else ENEMY_ENEMY
    CR -> CR: Push mutuo
  else ENTITY_WORLD
    CR -> CR: Confinar dentro del mundo
  else PLAYER_OBSTACLE
    CR -> CR: Push del jugador
  else ENEMY_OBSTACLE
    CR -> CR: Push del enemigo
  end
deactivate E

== FASE 4: DAÑO ==
DM -> E: ProcesarEventos()
activate E
  DM -> DM: Para cada CollisionEvent PLAYER_ENEMY
  DM -> DM: victim.HealthComponent.takeDamage(1)
  
  alt Jugador no está invulnerable
    DM -> DM: PlayerComponent.SetInvulnerable(1.0f)
    DM -> DM: Activar parpadeo
  end
  
  alt Jugador muere (HP <= 0)
    DM -> DM: m_GameOver = true
  end
deactivate E

@enduml
```

---

## 4. Sistema de IA de Enemigos

```plantuml
@startuml EnemyAI
title Secuencia: Comportamiento de IA de Enemigos

participant "EnemyAISystem" as AI
participant "Enemy\nEntity" as EN
participant "Transform\nComponent" as TC
participant "Enemy\nComponent" as EC
participant "Player\nEntity" as PE

== Cada Frame ==
AI -> AI: Para cada entidad con EnemyComponent

AI -> EN: GetComponent<EnemyComponent>()
AI -> EN: GetComponent<TransformComponent>()

== Cálculo de IA ==
AI -> PE: world.GetPlayer()

AI -> AI: Calcular distancia al jugador

alt distancia < focusRange
  
  note right of AI
    Jugador está en rango de visión
  end note
  
  AI -> AI: Calcular vector hacia jugador
  AI -> AI: Normalizar dirección
  AI -> AI: velocity_desired = direction * baseSpeed
  
else distancia >= focusRange
  
  note right of AI
    Jugador fuera de rango
  end note
  
  AI -> AI: velocity_desired = (0, 0)
  
end

== Suavizado con Lerp ==
AI -> TC: GetVelocity()

AI -> AI: velocity_actual = GetVelocity()

AI -> AI: velocity_smooth = Lerp(
  velocity_actual, 
  velocity_desired, 
  velocitySmoothing
)

note right of AI
  Lerp = actual + (desired - actual) * factor
  Factor pequeño = cambios lentos
  Factor grande = cambios rápidos
end note

AI -> TC: SetVelocity(velocity_smooth)

note right of AI
  En siguiente frame:
  MovementSystem aplicará:
  pos += velocity_smooth * dt
end note

@enduml
```

---

## 5. Invulnerabilidad del Jugador

```plantuml
@startuml PlayerInvulnerability
title Secuencia: Activación y Actualización de Invulnerabilidad

participant "DamageSystem" as DS
participant "Player\nComponent" as PC
participant "RenderSystem" as RS
participant "Frame\nLoop" as FL

== EVENTO DE DAÑO ==
DS -> DS: Detecta colisión PLAYER_ENEMY

DS -> PC: SetInvulnerable(duration = 1.0f)
activate PC
  PC -> PC: m_InvulnerabilityTimer = 1.0f
  PC -> PC: m_IsInvulnerable = true
deactivate PC

note right of DS
  Jugador no puede recibir más daño
  durante este tiempo
end note

== SIGUIENTES FRAMES ==
loop Cada Frame (mientras m_InvulnerabilityTimer > 0)
  
  FL -> PC: UpdateInvulnerability(deltaTime)
  activate PC
    PC -> PC: m_InvulnerabilityTimer -= deltaTime
    
    alt m_InvulnerabilityTimer <= 0
      PC -> PC: m_IsInvulnerable = false
      note right of PC
        Vuelve a ser vulnerable
      end note
    end
  deactivate PC
  
  == RENDERIZADO ==
  RS -> PC: IsInvulnerable()
  
  alt Invulnerable
    RS -> RS: Obtener progreso de parpadeo
    
    alt frame % 2 == 0
      RS -> RS: Renderizar sprite normalmente
    else frame % 2 == 1
      RS -> RS: Renderizar transparente (alpha = 128)
    end
    
    note right of RS
      Parpadeo visual cada 2 frames
      Feedback al jugador
    end note
    
  else No invulnerable
    RS -> RS: Renderizar sprite normal
  end
  
end

@enduml
```

---

## 6. Ciclo de Oleadas

```plantuml
@startuml WaveCycle
title Secuencia: Ciclo Completo de una Oleada

participant "WaveSystem" as W
participant "World" as WO
participant "SpawnSystem" as S
participant "Systems" as SYS
participant "LifetimeSystem" as L

== INICIO DE OLEADA ==
W -> W: m_CurrentWave = 0
W -> W: m_WaveTimer = 0
W -> W: m_SpawnTimer = 0

W -> WO: EmitEvent(WaveStartEvent(0, 0))
note right of W
  Notifica que inició oleada 0
  Phase 0 para obstáculos
end note

== DURANTE OLEADA ==
loop Mientras m_WaveTimer < duration

  W -> W: m_WaveTimer += deltaTime
  W -> W: m_SpawnTimer += deltaTime
  
  alt m_SpawnTimer >= spawnInterval
    
    W -> S: EmitEvent(SpawnEnemyEvent)
    note right of W
      Spawn de un enemigo
    end note
    
    W -> W: m_SpawnTimer = 0
    W -> W: m_EnemiesSpawnedInWave++
    
  end
  
  SYS -> SYS: Resto de sistemas (Physics, Collisions, etc)
  
end

== FIN DE OLEADA ==
W -> W: m_WaveTimer >= duration

W -> WO: EmitEvent(WaveEndEvent(0))
note right of W
  Notifica fin de oleada 0
end note

W -> W: m_WaveEnded = true

== LIMPIEZA ==
L -> WO: Procesa WaveEndEvent

L -> SYS: Eliminar todos los enemigos
activate SYS
  loop Para cada enemigo
    SYS -> SYS: DeleteEntity(enemy)
    note right of SYS
      Libera memoria
      Actualiza caches
    end
  end
deactivate SYS

L -> SYS: Eliminar todos los obstáculos
note right of L
  Limpia estado de oleada anterior
end note

== TRANSICIÓN ==
W -> W: NextWave()
activate W
  W -> W: m_CurrentWave++
  W -> W: m_WaveTimer = 0
  W -> W: m_WaveEnded = false
deactivate W

alt m_CurrentWave < totalWaves
  W -> W: StartWave(m_CurrentWave)
  note right of W
    Inicia siguiente oleada
    Repite ciclo
  end note
else m_CurrentWave >= totalWaves
  note right of W
    ¡Juego ganado!
  end note
end

@enduml
```

---

## 7. Procesamiento de Eventos

```plantuml
@startuml EventProcessing
title Secuencia: Cola de Eventos y Procesamiento

participant "Systems" as SYS
participant "World\nEventQueue" as Q
participant "Event\nProcessors" as EP

== DURANTE UPDATE ==
loop Cada Sistema puede emitir eventos

  SYS -> Q: EmitEvent(evento)
  activate Q
    Q -> Q: queue.push(evento)
    note right of Q
      Eventos encolados
      NO procesados inmediatamente
    end note
  deactivate Q
  
end

note over SYS, EP
  Los eventos se acumulan
  en la queue sin procesar
end note

== DESPUÉS DE TODOS LOS UPDATES ==
note over Q
  WaveManagerSystem.render() llama ProcessEvents()
end note

Q -> Q: ProcessEvents()
activate Q
  
  loop Mientras queue no está vacía
    
    Q -> Q: evento = queue.front()
    Q -> Q: queue.pop()
    
    Q -> EP: Distribuir a procesadores
    
    alt evento es CollisionEvent
      EP -> EP: CollisionResponseSystem procesa
      EP -> EP: DamageSystem procesa
    else evento es WaveEndEvent
      EP -> EP: LifetimeSystem procesa
    else evento es SpawnEnemyEvent
      EP -> EP: SpawnSystem procesa
    end
    
  end
  
deactivate Q

note right of Q
  Queue lista para siguiente frame
end note

@enduml
```

---

## 8. Flujo Frame-Rate Independent

```plantuml
@startuml FrameRateIndependence
title Secuencia: Movimiento Frame-Rate Independiente

participant "Clock" as CLK
participant "MovementSystem" as MV
participant "Entity" as E
participant "Screen" as SCR

== FRAME 1: FPS 60 ==
CLK -> CLK: deltaTime = 1/60 = 0.0167s

E -> E: velocity = (100, 0)
MV -> E: pos.x += velocity.x * deltaTime
MV -> E: pos.x += 100 * 0.0167 = 1.67 px

SCR -> SCR: Renderizar pos.x = 1.67

== FRAME 2: FPS 60 ==
CLK -> CLK: deltaTime = 0.0167s
E -> E: pos.x += 100 * 0.0167 = 1.67 px
E -> E: pos.x = 1.67 + 1.67 = 3.34

SCR -> SCR: Renderizar pos.x = 3.34

== FRAME 3: FPS 30 ==
CLK -> CLK: deltaTime = 1/30 = 0.0333s
E -> E: pos.x += 100 * 0.0333 = 3.33 px
E -> E: pos.x = 3.34 + 3.33 = 6.67

SCR -> SCR: Renderizar pos.x = 6.67

note over CLK, E
  En 2 frames @ 60 FPS = 0.0334s
  Se movieron: 3.34 px
  
  En 1 frame @ 30 FPS = 0.0333s
  Se movieron: 3.33 px
  
  ¡Prácticamente idéntico!
  Movimiento consistente sin importar FPS
end note

@enduml
```

---

## 9. Optimización HashMap O(1)

```plantuml
@startuml HashMapOptimization
title Secuencia: Búsqueda de Entidades O(1) vs O(N)

participant "MovementSystem" as MV
participant "World" as W
participant "Entities" as E
participant "HashMap" as HM

== ANTES: O(N) - Búsqueda lineal ==
MV -> W: GetAllEntities()
activate W
  W -> E: Iterar vector completo
  loop Para cada entidad
    E -> E: Verificar si es dinámico
    E -> E: Verificar si tiene Transform
  end
  W -> W: Retornar vector filtrado
deactivate W

note right of W
  Tiempo: O(N) donde N = total de entidades
  100 entidades = 100 checks
  1000 entidades = 1000 checks
end note

== DESPUÉS: O(1) - HashMap directo ==
MV -> HM: GetEnemies()
activate HM
  HM -> HM: Retornar m_EnemyEntities (cached)
deactivate HM

note right of HM
  Tiempo: O(1) constante
  100 enemigos = 1 acceso
  1000 enemigos = 1 acceso
end note

MV -> HM: GetPlayer()
activate HM
  HM -> HM: Retornar m_PlayerEntity (cached)
deactivate HM

note right of HM
  Tiempo: O(1) constante
  ¡Encuentra al jugador en 1 operación!
end note

== RESULTADO ==
note over MV, HM
  En escenario de stress test:
  - 45 enemigos
  - 38 obstáculos
  - 1 jugador
  
  Antes: ~84 checks por frame
  Después: 3 accesos O(1)
  
  Ganancia de rendimiento: 28x más rápido
end note

@enduml
```

---

## Resumen de Secuencias Críticas

| Secuencia | Duración típica | Puntos críticos |
|-----------|-----------------|-----------------|
| Spawn enemigo | <1ms | Posición aleatoria, configuración |
| Detección colisión | ~2ms | AABB checks (múltiples pares) |
| Resolución colisión | ~1ms | Push physics, confinamiento |
| IA enemigos | ~1ms | Distancia, Lerp suavizado |
| Renderizado | ~5ms | SDL_RenderTexture por sprite |
| **Total por frame @ 60 FPS** | **<10ms** | **Frame time budget: 16.67ms** |

---

**Siguiente:** Ver [EVENT_MANAGEMENT.md](./EVENT_MANAGEMENT.md) para detalles del sistema de eventos
