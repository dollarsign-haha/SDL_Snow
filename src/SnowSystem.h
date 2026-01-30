#pragma once
#include <SDL.h>
#include <vector>

struct Snowflake {
    float x;
    float y;
    float speed;
};

struct SnowConfig {
    int   maxFlakes = 800;     // 雪花数量
    float minSpeed = 80.0f;
    float maxSpeed = 200.0f;
    float wind = 0.0f;  // 风（负数向左）
};

class SnowSystem {
public:
    bool init(SDL_Renderer* renderer, int w, int h);
    void update(float dt);
    void render(SDL_Renderer* renderer);

private:
    void resetFlake(Snowflake& f);

private:
    int width;
    int height;

    SDL_Texture* snowTexture = nullptr;
    std::vector<Snowflake> flakes;
    SnowConfig config;
};
