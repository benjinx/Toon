Temporality
===
[![Build Status](https://ci.appveyor.com/api/projects/status/github/benjinx/Temporality?svg=true)](https://ci.appveyor.com/project/benjinx/Temporality)
[![Build Status](https://travis-ci.org/benjinx/Temporality.svg?branch=master)](https://travis-ci.org/benjinx/Temporality)

A C++ Game Engine, using OpenGL/GLSL designed to easily create AI, Gameplay, Graphics, Physics, and Networking demos.

Building
---

#### Windows

```
git clone https://github.com/benjinx/Temporality.git
cd Temporality
git submodule update --init
mkdir build
cd build
cmake ..
cmake --build .
```

Dependencies
---
* assimp
* glad
* glfw
* glm
* imgui
* openal
* sol2