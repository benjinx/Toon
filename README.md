Temporality
===
[![Build Status](https://ci.appveyor.com/api/projects/status/github/benjinx/Temporality?svg=true)](https://ci.appveyor.com/project/benjinx/Temporality)
[![Build Status](https://travis-ci.org/benjinx/Temporality.svg?branch=master)](https://travis-ci.org/benjinx/Temporality)

A C++ Game Engine, using OpenGL/GLSL designed to easily create AI, Gameplay, Graphics, Physics, and Networking demos.

Building
---

#### Linux && Windows

```
git clone https://github.com/benjinx/Temporality.git
cd Temporality
mkdir build && cd build
cmake ..
cmake --build .
```

Running Examples
---

The examples are built to run from their source directories. To facilitate this, there are convenience targets prefixed with `run-`.

### Example

```
cmake --build . --target run-HelloWorld
```

Dependencies
---
* assimp
* glfw
* glm
