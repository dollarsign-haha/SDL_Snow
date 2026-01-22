cmake_minimum_required(VERSION 3.16)

project(SDL_snow
    VERSION 1.0
    LANGUAGES CXX
)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# 查找 SDL2 和 SDL2_image
# （推荐配合 vcpkg 使用）
# --------------------------------------------------
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# --------------------------------------------------
# 源文件
# --------------------------------------------------
set(SOURCES
    src/Snow.cpp
    src/SnowSystem.cpp
)

set(HEADERS
    src/SnowSystem.h
)

# --------------------------------------------------
# 可执行文件
# --------------------------------------------------
add_executable(SDL_snow
    ${SOURCES}
    ${HEADERS}
)

# --------------------------------------------------
# 链接库
# --------------------------------------------------
target_link_libraries(SDL_snow
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
)

# --------------------------------------------------
# Windows 下隐藏控制台（可选）
# --------------------------------------------------
if (WIN32)
    set_target_properties(SDL_snow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
