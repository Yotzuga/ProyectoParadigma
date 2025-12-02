# 🎮 PROYECTO PARADIGMA

Un juego de acción tipo Mega Man desarrollado en **C++** con arquitectura **ECS (Entity Component System)**, utilizando SDL3 para renderizado y GLM para matemáticas.

## 📋 Descripción

PROYECTO es un juego donde el jugador debe:
- 🎯 **Sobrevivir oleadas de enemigos** progresivamente más difíciles
- 🔫 **Evitar obstáculos estáticos** en el camino
- 💚 **Gestionar su vida** (10 puntos de salud)
- 🌊 **Completar 5 oleadas** de combate

### Características principales

✨ **Arquitectura ECS moderna**
- 12 sistemas especializados para diferentes responsabilidades
- Componentes reutilizables y composables
- Separación clara entre lógica y datos

🎨 **Gráficos dinámicos**
- Fondos parallax cambiantes por fase
- 4 tipos de enemigos con comportamientos únicos
- Sprites animados y colisores visuales

🎮 **Gameplay progresivo**
- 5 oleadas con dificultad creciente
- 38 obstáculos distribuidos por fases
- Sistema de invulnerabilidad tras daño

⚡ **Rendimiento optimizado**
- HashMap O(1) para búsquedas de entidades
- Caching de estructuras dinámicas
- Logging mínimo para máxima eficiencia

---

## 🚀 Requisitos

### Software
- **Compilador**: GCC/G++ (C++17 o superior)
- **Build System**: Make
- **Sistema Operativo**: Windows (PowerShell) o Linux

### Librerías externas (incluidas en `/external/`)
- **SDL3** - Renderizado y manejo de eventos
- **SDL3_image** - Carga de texturas PNG/JPEG
- **GLM** - Matemáticas vectoriales
- **spdlog** - Logging (uso mínimo en producción)
- **nlohmann_json** - Parseo de configuración JSON

---

## 🏗️ Estructura del Proyecto

```
EXAMEN3/
├── bin/                    # Archivos compilados y ejecutable
│   └── main.exe           # Ejecutable del juego
├── include/
│   ├── *.h               # Headers de infraestructura
│   ├── Components/       # Headers de componentes (7 tipos)
│   └── Systems/          # Headers de sistemas (12 tipos)
├── src/
│   ├── *.cpp            # Implementación de infraestructura
│   ├── Components/      # Implementación de componentes
│   └── Systems/         # Implementación de sistemas
├── external/             # Librerías externas
│   ├── SDL3/
│   ├── SDL3_image/
│   ├── GLM/
│   ├── spdlog/
│   └── nlohmann_json/
├── assets/              # Recursos del juego
│   ├── sprites/         # Texturas PNG (player, enemigos, obstáculos)
│   ├── backgrounds/     # Fondos por fase
│   ├── music/           # Música y sonidos
│   └── config/          # Archivos de configuración JSON
├── Makefile             # Script de compilación
└── README.md            # Este archivo
```

---

## ⚙️ Compilación

### Build completo
```bash
cd "c:\Users\USER\Documents\VSCode-v3\USER\EXAMEN3"
make clean
make
```

### Build sin limpiar
```bash
make
```

### Ejecutar el juego
```bash
make run
```

### Limpiar archivos compilados
```bash
make clean
```

---

## 🎮 Controles del Juego

| Tecla | Acción |
|-------|--------|
| **W** | Mover arriba |
| **S** | Mover abajo |
| **A** | Mover izquierda |
| **D** | Mover derecha |
| **ESC** | Salir del juego |

---

## 🎯 Mecánicas de Juego

### El Jugador
- **Velocidad**: 400 px/s en 8 direcciones
- **Vida**: 10 puntos de salud
- **Invulnerabilidad**: 1 segundo tras recibir daño
- **Animación**: Parpadeo durante invulnerabilidad

### Enemigos (4 tipos)
```
Enemigo1: Velocidad moderada, rango medio
Enemigo2: Velocidad rápida, rango corto
Enemigo3: Velocidad lenta, rango largo
Enemigo4: Velocidad muy rápida, rango muy corto
```

Cada enemigo:
- Persigue al jugador cuando está en rango de visión
- Se desplaza suavemente (Lerp) para movimiento natural
- Muere en 1 golpe de colisión
- Se destruye al terminar la oleada

### Obstáculos
- **Estáticos**: No se mueven
- **Indestructibles**: No pueden ser eliminados
- **Cantidad**: 38 distribuidos en 5 fases
- **Colisión**: Empujan al jugador y enemigos

### Sistema de Oleadas
1. **Oleada 0**: 3 Enemigo1 + obstáculos fase 0
2. **Oleada 1**: 3 Enemigo2 + obstáculos fase 1
3. **Oleada 2**: 3 Enemigo3 + obstáculos fase 2
4. **Oleada 3**: 3 Enemigo4 + obstáculos fase 3
5. **Oleada 4**: Mix de todos + obstáculos fase 4

**Duración por oleada**: 8 segundos
**Intervalo de spawn**: 0.5 segundos entre enemigos

---

## 🏗️ Arquitectura ECS

### Componentes (7 tipos)
1. **TransformComponent** - Posición y velocidad
2. **SpriteComponent** - Textura y renderizado
3. **ColliderComponent** - Geometría AABB para colisiones
4. **HealthComponent** - HP y estado de muerte
5. **PlayerComponent** - Mecánicas específicas del jugador
6. **EnemyComponent** - Parámetros de IA
7. **ObstacleComponent** - Marcador de entidades estáticas

### Sistemas (12 tipos)

#### Input & Movement
- **PlayerInputSystem** - Lee teclado, calcula velocidad
- **MovementSystem** - Aplica velocidad con frame-rate independence

#### Physics & Collision
- **CollisionSystem** - Detecta colisiones AABB (5 tipos)
- **CollisionResponseSystem** - Resuelve colisiones (push/bounce/confine)

#### Logic & Behavior
- **EnemyAISystem** - Persecución inteligente con Lerp
- **DamageSystem** - Aplica daño, invulnerabilidad, game over
- **LifetimeSystem** - Limpia enemigos al terminar oleada

#### Rendering & UI
- **RenderSystem** - Renderiza sprites y mantiene timer
- **HUDSystem** - Muestra HP y tiempo en pantalla

#### Wave Management
- **WaveSystem** - Timeline de oleadas y fondos
- **SpawnSystem** - Crea enemigos y obstáculos
- **WaveManagerSystem** - Orquestador central

### Orden de Ejecución
```
1. WaveManagerSystem (orquestación + renderizado de fondos)
2. PlayerInputSystem (captura input)
3. EnemyAISystem (comportamiento enemigos)
4. MovementSystem (actualiza posiciones)
5. CollisionSystem (detección)
6. CollisionResponseSystem (resolución)
7. DamageSystem (daño + game over)
8. RenderSystem (sprites)
9. HUDSystem (interfaz)
```

---

## 📊 Resolución y Coordenadas

- **Resolución de pantalla**: 1920 x 1080 píxeles
- **Sistema de coordenadas**: (0,0) en esquina superior-izquierda
- **X+**: Derecha
- **Y+**: Abajo

### Tamaños de entidades
```
Jugador:     94 x 94 px
Enemigos:    28-36 x 36 px
Obstáculos:  100 x 100 px
```

---

## ⚡ Optimizaciones

### Búsqueda de Entidades O(1)
```cpp
// Antes: O(N) iteración completa
std::vector<Entity*> allEntities = world.GetAllEntities();

// Ahora: O(1) acceso directo
Entity* player = world.GetPlayer();
std::vector<Entity*> enemies = world.GetEnemies();
std::vector<Entity*> obstacles = world.GetObstacles();
```

### Caching de Sistemas
- **MovementSystem**: Usa caches en lugar de iterar todas las entidades
- **CollisionSystem**: Itera solo jugador, enemigos y obstáculos
- **RenderSystem**: Itera solo entidades con SpriteComponent

### Logging Mínimo
- Sin logs debug en bucle de juego
- Solo logs de error para condiciones críticas
- Salida limpia para máximo rendimiento

---

## 📝 Configuración (JSON)

### `settings.json`
```json
{
  "game": {
    "window_width": 1920,
    "window_height": 1080,
    "target_fps": 60
  }
}
```

### `Waves.json`
Define las 5 oleadas con:
- Número de enemigos por tipo
- Intervalo de spawn
- Duración total
- Multiplicador de velocidad

### `Enemy_entities.json`
Define 4 tipos de enemigos con:
- Velocidad base
- Rango de persecución
- Factor de suavizado (Lerp)

### `ObstaclesByFase.json`
Define posiciones de obstáculos:
- 5 fases (0-4)
- Coordenadas X,Y para cada obstáculo
- 38 obstáculos totales

---

## 🔄 Flujo del Juego

```
[INICIO]
   ↓
[Carga configuración + Crea jugador]
   ↓
[Inicia oleada 0]
   ↓
[LOOP PRINCIPAL]
   ├─ Input del jugador
   ├─ IA de enemigos
   ├─ Movimiento
   ├─ Colisiones
   ├─ Daño
   ├─ Renderizado
   └─ HUD
   ↓
[¿Jugador muere?] ──Sí→ [GAME OVER]
   ↓ No
[¿Oleada termina?] ──Sí→ [Limpia enemigos] → [Siguiente oleada]
   ↓ No
[Vuelta al LOOP]
```

---

## 🐛 Troubleshooting

### Problema: "No se ve el juego"
- Verifica que los archivos en `assets/sprites/` existan
- Comprueba resolución en `settings.json` (debe ser 1920x1080)

### Problema: "Muchos enemigos se solapan"
- Esto es normal, el sistema de colisiones los empuja automáticamente
- Aumenta `playerPushStrength` en Game.cpp si lo deseas

### Problema: "El juego se ve lento"
- Verifica que no tengas otros programas pesados corriendo
- Comprueba que estés compilando con `make` (no debug)

---

## 📚 Documentación del Código

Todos los headers contienen documentación Doxygen completa:
- `@class` - Descripción de clases
- `@brief` - Resumen de funcionalidad
- `@param` - Parámetros documentados
- `@return` - Valores de retorno explicados
- Ejemplos y notas cuando aplica

Para generar documentación HTML/PDF:
```bash
doxygen Doxyfile  # Si existe
```

---

## 🎓 Conceptos Utilizados

### Patrones de Diseño
- **Entity Component System (ECS)** - Separación datos/lógica
- **Event-Driven Architecture** - Comunicación entre sistemas
- **Object Pool** (implícito) - Reutilización de entidades
- **Strategy Pattern** - Diferentes comportamientos de IA

### Algoritmos
- **AABB Collision Detection** - Detección de colisiones rectangulares
- **Linear Interpolation (Lerp)** - Suavizado de movimiento
- **Spatial Hashing** (Vector de enemigos) - O(1) acceso a grupos

### C++ Features
- **Templates** - `GetComponent<T>()` genérico
- **Smart Pointers** - `std::unique_ptr` para gestión de memoria
- **STL Containers** - `std::vector`, `std::unordered_map`
- **Virtual Methods** - Polimorfismo en sistemas

---

## 📊 Estadísticas del Proyecto

| Métrica | Valor |
|---------|-------|
| Líneas de código C++ | ~3,500 |
| Número de clases | 20+ |
| Componentes | 7 |
| Sistemas | 12 |
| Archivos de configuración | 5 |
| Sprites + Fondos | 9 |

---

## 🔗 Tecnologías

- **Lenguaje**: C++17
- **Rendering**: SDL3 + SDL3_image
- **Matemáticas**: GLM
- **Logging**: spdlog
- **Configuración**: nlohmann_json
- **Compilador**: GCC/G++

---

## 📄 Licencia

Proyecto educativo - Libre para uso académico

---

## 👨‍💻 Autor

Desarrollado como proyecto de examen (EXAMEN3)

---

## ✅ Estado del Proyecto

- ✅ Arquitectura ECS completamente funcional
- ✅ 5 oleadas implementadas
- ✅ 4 tipos de enemigos con IA
- ✅ Sistema de colisiones robusto
- ✅ HUD con información en tiempo real
- ✅ Código bien documentado
- ✅ Optimizaciones de rendimiento
- ✅ Build system automatizado
- ✅ Listo para producción

---

**¡Que disfrutes el juego! 🎮**
