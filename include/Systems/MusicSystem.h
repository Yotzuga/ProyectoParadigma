#pragma once
#include <SDL3/SDL_audio.h>
#include <spdlog/spdlog.h>
#include <string>

/**
 * @class MusicSystem
 * @brief Sistema independiente para gestionar reproducción de música.
 *
 * Responsabilidades:
 * - Cargar archivo de música FLAC
 * - Reproducir música de fondo
 * - Detener música
 * - Limpiar recursos de audio
 *
 * Características:
 * - No tiene dependencias de Game (desacoplado)
 * - Sin control de volumen (reproductor simple)
 * - Reproducción continua sin pausa/resume
 */
class MusicSystem
{
private:
    SDL_AudioStream *m_MusicStream;  ///< Stream de reproducción de audio
    SDL_AudioDeviceID m_AudioDevice; ///< ID del dispositivo de audio
    Uint8 *m_AudioBuffer;            ///< Buffer de datos de audio
    Uint32 m_AudioLen;               ///< Tamaño del buffer
    Uint32 m_AudioPos;               ///< Posición actual de lectura
    bool m_IsPlaying;                ///< Flag de reproducción activa

public:
    // ===== CICLO DE VIDA =====
    /**
     * @brief Constructor del sistema de música
     */
    MusicSystem();

    /**
     * @brief Destructor que limpia recursos
     */
    ~MusicSystem();

    // ===== MÉTODOS =====
    /**
     * @brief Carga un archivo de música FLAC
     * @param filePath Ruta al archivo de música
     * @return true si la carga fue exitosa
     */
    bool LoadMusic(const std::string &filePath);

    /**
     * @brief Inicia la reproducción de música
     */
    void Play();

    /**
     * @brief Detiene la reproducción de música
     */
    void Stop();

    /**
     * @brief Verifica si la música está reproduciéndose
     * @return true si está activa
     */
    bool IsPlaying() const { return m_IsPlaying; }

    /**
     * @brief Limpia todos los recursos de audio
     */
    void Cleanup();
};
