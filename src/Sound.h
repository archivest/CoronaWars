#pragma once
#include "header.h"

class Sound
{
public:
    Sound()
    {
    }

    ~Sound()
    {
        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
    }

    void load(const std::string& wavFile)
    {
        SDL_LoadWAV(wavFile.c_str(), &wavSpec, &wavBuffer, &wavLength);
    }


    void openDevice()
    {
        deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    }

    void closeDevice()
    {
        SDL_CloseAudioDevice(deviceId);
    }

    void stop()
    {
        SDL_ClearQueuedAudio(deviceId);
    }

    void play()
    {
        int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        SDL_PauseAudioDevice(deviceId, 0);
    }

private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    SDL_AudioDeviceID deviceId;
};
