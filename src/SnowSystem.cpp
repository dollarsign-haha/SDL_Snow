#include "SnowSystem.h"
#include <SDL_image.h>
#include <cstdlib>

void SnowSystem::init(SDL_Renderer* renderer, int w, int h) {
    width = w;
    height = h;

    snowTexture = IMG_LoadTexture(renderer, "assets/snow_fat.png");

    flakes.resize(config.maxFlakes);
    for (auto& f : flakes) {
        resetFlake(f);
        f.y = static_cast<float>(rand() % height);
    }
}

void SnowSystem::resetFlake(Snowflake& f) {
    f.x = static_cast<float>(rand() % width);
    f.y = -static_cast<float>(rand() % height);
    f.speed = config.minSpeed +
        (rand() / (float)RAND_MAX) *
        (config.maxSpeed - config.minSpeed);
}

void SnowSystem::update(float dt) {
    for (auto& f : flakes) {
        f.y += f.speed * dt;
        f.x += config.wind * dt;

        if (f.y > height || f.x < -20 || f.x > width + 20) {
            resetFlake(f);
        }
    }
}

void SnowSystem::render(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.w = 8;
    dst.h = 8;

    for (auto& f : flakes) {
        dst.x = (int)f.x;
        dst.y = (int)f.y;
        SDL_RenderCopy(renderer, snowTexture, nullptr, &dst);
    }
}
