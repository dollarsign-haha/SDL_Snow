#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>     // getenv / setenv
#include "SnowSystem.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

static void log_cwd()
{
    char cwd[512];
#ifdef _WIN32
    _getcwd(cwd, sizeof(cwd));
#else
    getcwd(cwd, sizeof(cwd));
#endif
    SDL_Log("Current working directory: %s", cwd);
}

int main(int argc, char* argv[])
{
    /* =========================================================
       0. CI 环境检测（无 DISPLAY 时使用 dummy video driver）
       ========================================================= */
#ifndef _WIN32
    if (std::getenv("CI") && !std::getenv("DISPLAY")) {
        SDL_Log("CI environment detected, using dummy video driver");
        setenv("SDL_VIDEODRIVER", "dummy", 1);
    }
#endif

    /* =========================================================
       1. 初始化 SDL
       ========================================================= */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    /* =========================================================
       2. 初始化 SDL_image（PNG / JPG）
       ========================================================= */
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 2;
    }

    log_cwd();

    /* =========================================================
       3. 创建窗口（dummy driver 下不会真的显示）
       ========================================================= */
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 500;

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Snow Storm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 3;
    }

    /* =========================================================
       4. 创建 Renderer（dummy 下是 software renderer）
       ========================================================= */
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 4;
    }

    /* =========================================================
       5. 加载资源（CI 必须能正确失败）
       ========================================================= */
    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/bg.jpg");
    if (!bgTexture) {
        std::cerr << "IMG_LoadTexture failed: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 5;
    }

    /* =========================================================
       6. 初始化雪系统（真实逻辑）
       ========================================================= */
    SnowSystem snow;
    if (!snow.init(renderer, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        std::cerr << "SnowSystem init failed" << std::endl;
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 6;
    }

    /* =========================================================
       7. 主循环（CI 跑固定帧数，本地可交互）
       ========================================================= */
    bool running = true;
    SDL_Event event;

    Uint32 lastTicks = SDL_GetTicks();
    int frameCount = 0;
    const int CI_MAX_FRAMES = 120; // CI 中跑 120 帧 ≈ 2 秒

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        Uint32 currentTicks = SDL_GetTicks();
        float dt = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        snow.update(dt);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        snow.render(renderer);
        SDL_RenderPresent(renderer);

#ifndef _WIN32
        if (std::getenv("CI")) {
            frameCount++;
            if (frameCount >= CI_MAX_FRAMES) {
                SDL_Log("CI frame limit reached, exiting cleanly");
                break;
            }
        }
#endif
    }

    /* =========================================================
       8. 清理
       ========================================================= */
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    SDL_Log("Program exited normally");
    return 0;
}
