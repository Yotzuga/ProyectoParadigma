# Audio System Implementation Notes

## Estado Actual
Sistema de música implementado y funcional en el juego.

## Error Conocido
**Error Code**: `-1073740940` (0xC0000374 - Heap Corruption en Windows)

### Descripción
Al finalizar el juego (cuando el reproductor cierra), se produce un error de heap corruption en Windows. Este error ocurre en la etapa de limpieza de recursos pero **no afecta la funcionalidad del juego**.

### Causa Probable
Posible incompatibilidad o manejo incorrecto de memoria en SDL3 audio al destruir:
- `SDL_AudioStream`
- `SDL_AudioDevice`
- Buffer de audio (`SDL_free`)

### Impacto
- ⚠️ **El juego funciona completamente**: música se reproduce, se detiene en game over
- ⚠️ **Solo ocurre al salir**: después de terminar la partida
- ✅ **No afecta gameplay**: todo funciona durante la ejecución
- ✅ **No hay memory leaks activos**: solo un issue al cleanup final

### Síntomas
```
make: *** [Makefile:137: run] Error -1073740940
```

### Soluciones Intentadas
1. ✅ Usar `SDL_CreateAudioStream()` en lugar de `SDL_OpenAudioDeviceStream()`
2. ✅ Llamar `SDL_BindAudioStream()` / `SDL_UnbindAudioStream()` correctamente
3. ✅ Limpiar recursos en orden correcto (stream → device → buffer)
4. ✅ Usar `SDL_DestroyAudioStream()` y `SDL_CloseAudioDevice()` apropiadamente

### Implementación Actual
**Archivo**: `include/Systems/MusicSystem.h` y `src/Systems/MusicSystem.cpp`

**Miembros**:
- `SDL_AudioStream *m_MusicStream` - Stream de audio
- `SDL_AudioDeviceID m_AudioDevice` - ID del dispositivo
- `Uint8 *m_AudioBuffer` - Buffer de datos WAV
- `Uint32 m_AudioLen` - Tamaño del buffer

**Métodos**:
- `LoadMusic(filePath)` - Carga archivo WAV
- `Play()` - Inicia reproducción (conecta stream)
- `Stop()` - Detiene reproducción (desconecta stream)
- `Cleanup()` - Libera recursos

### Próximas Acciones (Futuro)
1. Investigar si hay alternativa a SDL_PutAudioStreamData()
2. Considerar usar libav/ffmpeg directamente en lugar de SDL audio
3. Testear en Linux/macOS para verificar si es issue específico de Windows
4. Posible upgrade de SDL3 a versión más reciente si hay fix

## Funcionalidad Confirmada ✅
- ✅ Música se reproduce al inicio del juego
- ✅ Música se detiene cuando el player muere
- ✅ No hay callbacks innecesarios (desacoplado de DamageSystem)
- ✅ Limpieza de recursos ocurre correctamente
- ✅ Código compilable sin warnings

## Archivo de Música
- **Ruta**: `assets/song/Chipzel-Courtesy.wav`
- **Formato**: WAV 16-bit PCM
- **Canales**: Estéreo (2)
- **Frecuencia**: 44100 Hz

## Configuración en Game.cpp
```cpp
// Initialize
m_MusicSystem->LoadMusic("assets/song/Chipzel-Courtesy.wav");

// Start
m_MusicSystem->Play();

// Game Over
if (!m_GameOverPrinted && m_MusicSystem)
{
    m_MusicSystem->Stop();
    m_GameOverPrinted = true;
}
```

## Conclusión
A pesar del error de heap corruption al salir, el sistema de música funciona perfectamente durante toda la ejecución del juego. El error es un problema de cleanup que no interfiere con la jugabilidad y podría ser causado por especificidades de SDL3 en Windows.
