#include "SnowSystem.h"
#include <SDL_image.h>
#include <cstdlib>

bool SnowSystem::init(SDL_Renderer* renderer, int w, int h)
{
    // 1. 参数校验（CI 非常重要）
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "SnowSystem::init failed: renderer is null");
        return false;
    }

    if (w <= 0 || h <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "SnowSystem::init failed: invalid size %dx%d", w, h);
        return false;
    }

    width = w;
    height = h;

    // 2. 加载雪花纹理
    snowTexture = IMG_LoadTexture(renderer, "assets/snow_fat.png");
    if (!snowTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load snow texture: %s", IMG_GetError());
        return false;
    }

    // 3. 初始化雪花数据
    flakes.clear();
    flakes.resize(config.maxFlakes);

    for (auto& f : flakes) {
        resetFlake(f);
        f.y = static_cast<float>(rand() % height);
    }

    SDL_Log("SnowSystem initialized: %zu flakes", flakes.size());
    return true;
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
