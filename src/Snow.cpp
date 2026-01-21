#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "SnowSystem.h"
#include <direct.h>   // _getcwd

int main(int argc, char* argv[])
{
    // 1. 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. 初始化 SDL_image（JPG / PNG）
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        return 1;
    }

    // 3. 创建窗口
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 501;

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Snow Storm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "CreateWindow failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 4. 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "CreateRenderer failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 5. 加载背景图
    char cwd[512];
    _getcwd(cwd, sizeof(cwd));
    SDL_Log("Current directory: %s", cwd);
    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/bg.jpg");
    if (!bgTexture) {
        std::cerr << "Load bg.jpg failed: " << IMG_GetError() << std::endl;
        return 1;
    }

    // 6. 初始化雪系统
    SnowSystem snow;
    snow.init(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    // 7. 主循环
    bool running = true;
    SDL_Event event;

    Uint32 lastTicks = SDL_GetTicks();

    while (running) {
        // --- 事件处理 ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // --- 计算 delta time ---
        Uint32 currentTicks = SDL_GetTicks();
        float dt = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        // --- 更新 ---
        snow.update(dt);

        // --- 渲染 ---
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        snow.render(renderer);

        SDL_RenderPresent(renderer);
    }

    // 8. 释放资源
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
