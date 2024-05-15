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
            std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        }
    }

    ~AudioManager() {
        // Quit SDL_mixer
        Mix_CloseAudio();
    }

    void load_sound(const std::filesystem::path &filePath) {
        Mix_Chunk *chunk = Mix_LoadWAV(filePath.string().c_str());
        if (chunk == nullptr) {
            // Handle error with explanation
            std::cerr << "Failed to load sound: because " << Mix_GetError() << std::endl;
            return;
        }

        std::string name = filePath.stem().string();

        std::cout << "Loaded sound: " << name << std::endl;

        sounds[name] = std::shared_ptr<Mix_Chunk>(chunk, Mix_FreeChunk);
    }

    void play_sound(const std::string &name) {
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