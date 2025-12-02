# 📚 Documentación Completa del Proyecto - EXAMEN3

Bienvenido a la documentación técnica de EXAMEN3. Este proyecto implementa un juego tipo Mega Man usando **C++ con arquitectura ECS (Entity Component System)**.

---

## 🎯 Índice de Documentación

### 1. **[ARCHITECTURE.md](./ARCHITECTURE.md)** - Arquitectura General
   - Visión general del proyecto
   - Diagrama de clases con PlantUML
   - Patrón ECS explicado
   - Flujo de eventos
   - Ciclo update-render
   - Patrones de diseño utilizados
   
   **Ideal para:** Entender la estructura general del proyecto

### 2. **[SEQUENCE_DIAGRAMS.md](./SEQUENCE_DIAGRAMS.md)** - Diagramas de Secuencia
   - Inicio del juego
   - Spawn de enemigos
   - Sistema de colisiones
   - IA de enemigos con Lerp
   - Invulnerabilidad del jugador
   - Ciclo de oleadas
   - Procesamiento de eventos
   - Frame-rate independence
   - Optimización HashMap O(1)
   
   **Ideal para:** Ver cómo interactúan los sistemas paso a paso

### 3. **[EVENT_MANAGEMENT.md](./EVENT_MANAGEMENT.md)** - Manejo de Eventos
   - Sistema de eventos event-driven
   - Arquitectura de eventos
   - Tipos de eventos definidos
   - Ciclo de vida de eventos
   - Desacoplamiento entre sistemas
   - Ejemplos prácticos completos
   - Desafíos resueltos
   
   **Ideal para:** Entender cómo se comunican los sistemas sin acoplamiento

### 4. **[UPDATE_RENDER_CYCLE.md](./UPDATE_RENDER_CYCLE.md)** - Ciclo Update-Render
   - Conceptos fundamentales
   - Game loop clásico
   - Implementación en EXAMEN3
   - Frame-rate independence con deltaTime
   - Timing y sincronización
   - Problemas comunes y soluciones
   - Performance profiling
   
   **Ideal para:** Entender cómo funciona el ciclo principal del juego

### 5. **[PATTERNS.md](./PATTERNS.md)** - Patrones de Diseño
   - Entity Component System (ECS)
   - Event-Driven Architecture
   - Object Pool
   - Strategy Pattern
   - Template Method Pattern
   - Factory Pattern
   - Comparación antes/después
   
   **Ideal para:** Aprender patrones aplicados en el proyecto

---

## 📋 README del Proyecto

Para información básica sobre compilación, requisitos y controles, ver:
- **[README.md](../README.md)** - Descripción general, build, controles

---

## 🗂️ Estructura de Archivos

```
EXAMEN3/
├── README.md                      # Información general (compilación, controles)
├── ARCHITECTURE.md                # Arquitectura ECS + Diagramas de clases
├── SEQUENCE_DIAGRAMS.md           # Diagramas de secuencia (PlantUML)
├── EVENT_MANAGEMENT.md            # Sistema de eventos detallado
├── UPDATE_RENDER_CYCLE.md         # Ciclo update-render + timing
├── PATTERNS.md                    # Patrones de diseño
│
├── include/                       # Headers documentados con Doxygen
│   ├── Entity.h
│   ├── Component.h
│   ├── ISystem.h
│   ├── World.h
│   ├── Event.h
│   ├── Game.h
│   ├── Components/
│   │   ├── TransformComponent.h
│   │   ├── SpriteComponent.h
│   │   ├── ColliderComponent.h
│   │   ├── HealthComponent.h
│   │   ├── PlayerComponent.h
│   │   ├── EnemyComponent.h
│   │   └── ObstacleComponent.h
│   └── Systems/
│       ├── PlayerInputSystem.h
│       ├── MovementSystem.h
│       ├── CollisionSystem.h
│       ├── CollisionResponseSystem.h
│       ├── DamageSystem.h
│       ├── EnemyAISystem.h
│       ├── RenderSystem.h
│       ├── HUDSystem.h
│       ├── LifetimeSystem.h
│       ├── SpawnSystem.h
│       ├── WaveSystem.h
│       └── WaveManagerSystem.h
│
├── src/                           # Implementaciones
│   └── ... (espejo de include)
│
├── assets/                        # Recursos del juego
│   ├── sprites/
│   ├── backgrounds/
│   ├── music/
│   └── config/
│
└── doc/                          # Documentación (este archivo)
```

---

## 🎓 Guía de Lectura Recomendada

### Para Principiantes

1. **Comienza con:** README.md → Compilar y ejecutar el juego
2. **Luego lee:** ARCHITECTURE.md → Entender la estructura
3. **Después:** SEQUENCE_DIAGRAMS.md → Ver cómo funciona
4. **Finalmente:** EVENT_MANAGEMENT.md + UPDATE_RENDER_CYCLE.md → Detalles

### Para Desarrolladores Experimentados

1. **ARCHITECTURE.md** - Diagramas de clases y patrones
2. **SEQUENCE_DIAGRAMS.md** - Interacciones críticas
3. **EVENT_MANAGEMENT.md** - Desacoplamiento
4. **UPDATE_RENDER_CYCLE.md** - Timing y sincronización

### Para Extender el Proyecto

1. Revisar **PATTERNS.md** para entender patrones usados
2. Leer headers relevantes en `include/`
3. Seguir convenciones establecidas
4. Documentar cambios con comentarios Doxygen

---

## 🔑 Conceptos Clave

### Entity Component System (ECS)
- **Separación:** Datos (Components) vs Lógica (Systems)
- **Flexibilidad:** Combinar components sin herencia
- **Rendimiento:** Mejor cache locality y acceso O(1)

### Event-Driven Architecture
- **Desacoplamiento:** Sistemas independientes
- **Comunicación:** A través de eventos encolados
- **Escalabilidad:** Fácil agregar nuevos sistemas

### Frame-Rate Independence
- **Fórmula:** `position += velocity * deltaTime`
- **Ventaja:** Comportamiento consistente a cualquier FPS
- **Resultado:** Juego suave a 30, 60, 120 FPS

### Optimizaciones O(1)
- **HashMap:** Búsquedas rápidas de entidades
- **Caching:** Listas separadas por tipo
- **Mejora:** 35% speedup en stress test

---

## 📊 Estadísticas del Proyecto

| Métrica | Valor |
|---------|-------|
| Líneas de código C++ | ~3,500 |
| Clases principales | 20+ |
| Componentes ECS | 7 |
| Sistemas ECS | 12 |
| Tipos de eventos | 8 |
| Enemigos tipos | 4 |
| Oleadas | 5 |
| Obstáculos totales | 38 |
| Resolución | 1920x1080 |
| FPS objetivo | 60 |

---

## 🚀 Quick Start

```bash
# 1. Compilar
cd EXAMEN3
make clean
make

# 2. Ejecutar
make run

# 3. Jugar
# Controles: WASD para mover, ESC para salir
```

---

## 🏆 Retos Principales Resueltos

### 1. Coordinación Update-Render ✅
**Desafío:** Sincronizar lógica y renderizado sin estados inconsistentes

**Solución:** Separar fases, procesamiento ordenado de sistemas, eventos post-render

**Documentación:** [UPDATE_RENDER_CYCLE.md](./UPDATE_RENDER_CYCLE.md)

---

### 2. Manejo de Eventos ✅
**Desafío:** Comunicar entre sistemas sin acoplamiento directo

**Solución:** Cola de eventos, procesamiento centralizado, desacoplamiento total

**Documentación:** [EVENT_MANAGEMENT.md](./EVENT_MANAGEMENT.md)

---

### 3. Frame-Rate Independence ✅
**Desafío:** Comportamiento consistente a diferentes FPS

**Solución:** Multiplicar por deltaTime en movimiento

**Documentación:** [UPDATE_RENDER_CYCLE.md - Frame-Rate Independence](./UPDATE_RENDER_CYCLE.md#frame-rate-independence)

---

### 4. Rendimiento Bajo Carga ✅
**Desafío:** 45 enemigos + 38 obstáculos sin laggy

**Solución:** HashMap O(1), caching, optimizaciones de colisiones

**Documentación:** [SEQUENCE_DIAGRAMS.md - HashMap O(1)](./SEQUENCE_DIAGRAMS.md#9-optimización-hashmap-o1)

---

### 5. Arquitectura Escalable ✅
**Desafío:** Agregar nuevos componentes/sistemas sin romper existentes

**Solución:** ECS + Factory Pattern + Template Method

**Documentación:** [ARCHITECTURE.md - Patrones de Diseño](./ARCHITECTURE.md#patrones-de-diseño)

---

## 📖 Diagramas PlantUML

Todos los diagramas en esta documentación están en **PlantUML**, formato texto que puede ser:

- **Renderizado en GitHub** (automático)
- **Importado en PlantUML Online** (http://www.plantuml.com/plantuml/uml/)
- **Convertido a PNG/SVG/PDF** con herramientas PlantUML

**Tipos de diagramas usados:**
- Class diagrams (estructura)
- Sequence diagrams (interacciones)
- Component diagrams (arquitectura)

---

## 🔗 Referencias Externas

- **PlantUML:** http://www.plantuml.com/
- **Entity Component System:** https://en.wikipedia.org/wiki/Entity_component_system
- **SDL3:** https://www.libsdl.org/
- **GLM:** https://github.com/g-truc/glm
- **Game Programming Patterns:** https://gameprogrammingpatterns.com/

---

## 👨‍💻 Autor

Desarrollado como proyecto de examen (EXAMEN3)

- **Lenguaje:** C++17
- **Rendering:** SDL3
- **Arquitectura:** ECS
- **Patrones:** Event-Driven, Factory, Strategy

---

## 📝 Notas Importantes

### Sobre la Documentación

1. **Completa:** Cubre arquitectura, eventos, timing, patrones
2. **Visual:** Diagramas PlantUML en cada documento
3. **Práctica:** Ejemplos de código real del proyecto
4. **Pedagógica:** Explica el "por qué" no solo el "cómo"

### Sobre el Código

1. **Documentado:** Headers con comentarios Doxygen
2. **Limpio:** Sin logs innecesarios en producción
3. **Optimizado:** O(1) búsquedas, caching inteligente
4. **Extensible:** Fácil agregar nuevas entidades/sistemas

### Sobre el Rendimiento

1. **60 FPS objetivo:** Consistentemente logrado
2. **Presupuesto:** ~16.67ms por frame
3. **Headroom:** 33% de buffer disponible
4. **Stress test:** 45 enemigos + 38 obstáculos = suave

---

## ❓ Preguntas Frecuentes

**P: ¿Por qué ECS y no herencia de clases?**
R: ECS permite composición flexible sin problemas de herencia profunda

**P: ¿Por qué procesar eventos después de render?**
R: Evita race conditions y mantiene estado consistente

**P: ¿Por qué multiplicar por deltaTime?**
R: Garantiza comportamiento independiente del FPS

**P: ¿Cómo agregar un nuevo tipo de enemigo?**
R: Ver [PATTERNS.md - Strategy Pattern](./PATTERNS.md#strategy-pattern)

---

## 🎯 Próximos Pasos

1. **Compilar:** `make` en directorio raíz
2. **Ejecutar:** `make run`
3. **Explorar código:** Ver `include/` para entender estructura
4. **Leer documentación:** Seguir guía de lectura arriba
5. **Extender:** Agregar nuevos componentes/sistemas

---

**¡Bienvenido a EXAMEN3! 🎮**

Esperamos que disfrutes explorando la arquitectura ECS y aprendas los patrones de diseño utilizados. La documentación está aquí para ayudarte a entender cada aspecto del proyecto.

Para preguntas específicas, consulta el documento relacionado en la tabla arriba.
