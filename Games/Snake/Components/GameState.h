#pragma once

#include "ECS/Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Game State component
 * Tracks the current state of the Snake game
 */
class GameState : public Component {
public:
    enum State {
        MENU = 0,
        PLAYING = 1,
        PAUSED = 2,
        GAME_OVER = 3
    };
    
    State currentState;
    int score;
    int highScore;
    int level;
    float gameSpeed; // Base game speed multiplier
    bool isNewHighScore;
    
    GameState() 
        : currentState(PLAYING), score(0), highScore(0), level(1), 
          gameSpeed(1.0f), isNewHighScore(false) {}
    
    // Component interface
    Component* Clone() const override {
        return new GameState(*this);
    }
    
    const char* GetTypeName() const override {
        return "GameState";
    }
    
    static const char* GetTypeNameStatic() {
        return "GameState";
    }
    
    // Helper functions
    void AddScore(int points) {
        score += points;
        if (score > highScore) {
            highScore = score;
            isNewHighScore = true;
        }
    }
    
    void ResetGame() {
        score = 0;
        level = 1;
        gameSpeed = 1.0f;
        isNewHighScore = false;
        currentState = PLAYING;
    }
    
    void NextLevel() {
        level++;
        gameSpeed += 0.1f; // Increase speed each level
    }
    
    const char* GetStateString() const {
        switch (currentState) {
            case MENU: return "MENU";
            case PLAYING: return "PLAYING";
            case PAUSED: return "PAUSED";
            case GAME_OVER: return "GAME_OVER";
            default: return "UNKNOWN";
        }
    }
};

} // namespace ECS
} // namespace Lite2D
