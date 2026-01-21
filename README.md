❄️ SDL Snow Demo

一个使用 SDL2 + C++ 实现的 城市暴风雪特效示例项目。
项目以清晰结构、易扩展设计为目标，适合 SDL / 图形初学者学习，也适合作为特效系统的基础模板。

✨ Features

🌨 真实暴风雪效果

大雪花

高速下落

强风横向漂移

🏙 城市背景支持

可加载 JPG / PNG 背景图（如外滩、城市夜景）

🧩 模块化设计

SnowSystem 独立封装

主循环极简，方便扩展

⚙️ 基于 SDL2 / SDL2_image

🪟 支持 Windows + Visual Studio 2019 (x64)

📂 Project Structure
SDL_snow/
│
├── src/
│   ├── Snow.cpp          # 程序入口
│   ├── SnowSystem.h      # 雪系统接口
│   └── SnowSystem.cpp    # 雪系统实现
│
├── assets/
│   ├── bg.jpg            # 城市背景图
│   └── snow.png          # 暴雪用大雪花纹理（透明 PNG）
│
├── docs/
│   └── ShSnowing.gif       # 运行效果预览（可选）
│
├── README.md
└── .gitignore

🧰 Dependencies

SDL2 (2.32.x)

SDL2_image

JPEG / PNG support

Windows x64

Visual Studio 2019

项目未使用 OpenGL，纯 SDL2 Renderer，配置简单。

🛠 Build & Run (Windows / VS2019)
1️⃣ 克隆仓库
git clone https://github.com/dollarsign-haha/SDL_Snow.git

2️⃣ 准备依赖 DLL

确保以下 DLL 与 exe 位于同一目录（如 x64/Debug）：

SDL2.dll
SDL2_image.dll
libjpeg-9.dll
zlib1.dll

3️⃣ 资源目录

运行目录下需要有：

assets/
 ├── bg.jpg
 └── snow.png

4️⃣ 运行

直接在 Visual Studio 中：

Debug → x64 → 本地 Windows 调试器

🎮 Controls
操作	功能
ESC	退出程序
🧠 Design Overview
SnowSystem 架构
SnowSystem
 ├── Snowflake (单个雪花)
 ├── SnowConfig (参数配置)
 ├── update(dt)
 └── render(renderer)

主循环极简
SnowSystem snow;
snow.init(renderer, width, height);

while (running) {
    snow.update(dt);
    snow.render(renderer);
}

🔧 Easy Customization
🌪 暴风雪参数
config.maxFlakes = 1200;
config.wind      = -200.0f;
config.minSpeed  = 150.0f;
config.maxSpeed  = 300.0f;

❄ 不同大小雪花
int size = 6 + rand() % 10;
dst.w = dst.h = size;

🌫 风随时间变化
config.wind = sin(SDL_GetTicks() * 0.001f) * 250.0f;