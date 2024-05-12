#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

class AudioManager {
public:
    AudioManager() {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            // Handle error
        }
    }

    ~AudioManager() {
        // Quit SDL_mixer
        Mix_CloseAudio();
    }

    void LoadSound(const std::string& filePath, const std::string& name){
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (chunk == nullptr) {
            // Handle error with explanation
            std::cerr << "Failed to load sound: because " << Mix_GetError() << std::endl;
            return;
        }
        sounds[name] = std::shared_ptr<Mix_Chunk>(chunk, Mix_FreeChunk);
    }

    void PlaySound(const std::string& name) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            Mix_PlayChannel(-1, it->second.get(), 0);
        } else {
            // Handle error
            std::cerr << "Sound not found: " << name << std::endl;
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> sounds;
};