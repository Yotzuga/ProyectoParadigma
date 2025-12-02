#include "../../include/Systems/MusicSystem.h"
#include <cstring>

MusicSystem::MusicSystem()
    : m_MusicStream(nullptr), m_AudioDevice(0), m_AudioBuffer(nullptr),
      m_AudioLen(0), m_AudioPos(0), m_IsPlaying(false)
{
}

MusicSystem::~MusicSystem()
{
    Cleanup();
}

bool MusicSystem::LoadMusic(const std::string &filePath)
{
    spdlog::info("Intentando cargar música desde: {}", filePath);

    // Limpiar si ya hay algo cargado
    Cleanup();

    // Cargar archivo de audio (solo WAV con SDL_LoadWAV)
    SDL_AudioSpec spec;
    Uint8 *audioBuf = nullptr;
    Uint32 audioLen = 0;

    if (!SDL_LoadWAV(filePath.c_str(), &spec, &audioBuf, &audioLen))
    {
        spdlog::error("No se pudo cargar la música desde: {}", filePath);
        spdlog::error("SDL Error: {}", SDL_GetError());
        return false;
    }

    spdlog::info("✓ Archivo de música cargado: {} bytes", audioLen);
    spdlog::info("  Formato: {} canales, {} Hz", spec.channels, spec.freq);

    // Guardar buffer en miembros de la clase
    m_AudioBuffer = audioBuf;
    m_AudioLen = audioLen;
    m_AudioPos = 0;

    // Crear stream de audio (sin dispositivo específico)
    m_MusicStream = SDL_CreateAudioStream(&spec, &spec);
    if (!m_MusicStream)
    {
        spdlog::error("No se pudo crear stream de audio: {}", SDL_GetError());
        SDL_free(m_AudioBuffer);
        m_AudioBuffer = nullptr;
        return false;
    }

    // Cargar datos en el stream
    if (!SDL_PutAudioStreamData(m_MusicStream, m_AudioBuffer, m_AudioLen))
    {
        spdlog::error("No se pudo cargar datos en stream: {}", SDL_GetError());
        SDL_DestroyAudioStream(m_MusicStream);
        m_MusicStream = nullptr;
        SDL_free(m_AudioBuffer);
        m_AudioBuffer = nullptr;
        return false;
    }

    // Abrir dispositivo de audio DEFAULT
    m_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (m_AudioDevice == 0)
    {
        spdlog::error("No se pudo abrir dispositivo de audio: {}", SDL_GetError());
        SDL_DestroyAudioStream(m_MusicStream);
        m_MusicStream = nullptr;
        SDL_free(m_AudioBuffer);
        m_AudioBuffer = nullptr;
        return false;
    }

    spdlog::info("✓ Música cargada en memoria exitosamente");
    return true;
}

void MusicSystem::Play()
{
    if (!m_MusicStream || m_AudioDevice == 0)
    {
        spdlog::warn("No hay música cargada para reproducir");
        return;
    }

    if (m_IsPlaying)
    {
        return; // Ya está reproduciendo
    }

    // Conectar stream al dispositivo
    if (!SDL_BindAudioStream(m_AudioDevice, m_MusicStream))
    {
        spdlog::error("No se pudo conectar stream a dispositivo: {}", SDL_GetError());
        return;
    }

    // Reanudar reproducción del dispositivo
    SDL_ResumeAudioDevice(m_AudioDevice);

    m_IsPlaying = true;
    spdlog::info("✓ Música iniciada");
}

void MusicSystem::Stop()
{
    if (m_AudioDevice == 0)
    {
        return;
    }

    if (!m_IsPlaying)
    {
        return; // Ya está detenida
    }

    // Desconectar stream del dispositivo
    if (m_MusicStream)
    {
        SDL_UnbindAudioStream(m_MusicStream);
    }

    // Pausar dispositivo de audio
    SDL_PauseAudioDevice(m_AudioDevice);

    m_IsPlaying = false;
    spdlog::info("Música detenida");
}

void MusicSystem::Cleanup()
{
    Stop();

    if (m_MusicStream)
    {
        SDL_DestroyAudioStream(m_MusicStream);
        m_MusicStream = nullptr;
    }

    if (m_AudioDevice != 0)
    {
        SDL_CloseAudioDevice(m_AudioDevice);
        m_AudioDevice = 0;
    }

    if (m_AudioBuffer)
    {
        SDL_free(m_AudioBuffer);
        m_AudioBuffer = nullptr;
    }

    m_AudioLen = 0;
    m_AudioPos = 0;

    spdlog::info("Recursos de música limpiados");
}
