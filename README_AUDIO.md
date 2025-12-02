# Configuración de Música para EXAMEN3

## Problema: SDL3 solo soporta formato WAV nativamente

El sistema de música requiere archivos en formato **WAV** para funcionar correctamente con SDL3.

## Solución: Convertir FLAC a WAV

### Opción 1: Usar FFmpeg (Recomendado)

Si tienes FFmpeg instalado:

```bash
ffmpeg -i "Chipzel - Courtesy.flac" -acodec pcm_s16le -ar 44100 "Chipzel - Courtesy.wav"
```

**Parámetros:**
- `-acodec pcm_s16le`: Codec PCM 16-bit little-endian (estándar WAV)
- `-ar 44100`: Frecuencia de muestreo 44100 Hz (CD quality)

### Opción 2: Usar Audacity (Interfaz gráfica)

1. Abrir Audacity
2. File → Open → Seleccionar archivo FLAC
3. File → Export → Export as WAV
4. Guardar en `assets/music/`

### Opción 3: Usar Online Converter

1. Ir a https://online-convert.com/
2. Seleccionar "Audio Converter"
3. Subir archivo FLAC
4. Elegir WAV como formato de salida
5. Convertir y descargar

## Ubicación del archivo

Una vez convertido, colocar el archivo WAV en:

```
assets/music/Chipzel - Courtesy.wav
```

El código automáticamente lo detectará y cargará.

## Verificar que funciona

1. Compilar el proyecto: `make clean && make`
2. Ejecutar: `make run`
3. Debería ver en los logs:
   - ✓ Archivo de música cargado: XXXX bytes
   - ✓ Música cargada en memoria exitosamente
   - ✓ Música iniciada

4. Al jugar, debería escuchar música de fondo
5. Al morir (Game Over), la música se detiene

## Especificaciones soportadas

| Formato | Soportado |
|---------|-----------|
| WAV (PCM) | ✅ Sí |
| FLAC | ❌ No (convertir a WAV) |
| MP3 | ⚠️ Posiblemente (no testeado) |
| OGG | ⚠️ Posiblemente (no testeado) |

## Troubleshooting

### "No se pudo cargar la música"
- Verificar que el archivo esté en `assets/music/`
- Verificar que sea WAV (no FLAC)
- Comprobar que el nombre sea exacto: `Chipzel - Courtesy.wav`

### "Música no se escucha"
- Verificar volumen del sistema
- Verificar que SDL3 tenga acceso al dispositivo de audio
- Ver logs para errores específicos

### "SDL_OpenAudioDeviceStream falló"
- Verificar que el dispositivo de audio esté conectado
- Reiniciar la aplicación
- Verificar drivers de audio del sistema
