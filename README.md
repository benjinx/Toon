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
* glfw
* glm


## Contributing

### Pull Requests

Use the "Branch and Pull Request" model for contributing changes.

### Commit Message Format

```
<type>(<scope>): <title>
<message>
```

**Allowed `<type>`**

* `Release`: A new release, increments Major
* `Feat`: A new feature, increments Minor
* `Fix`: A small fix, increments Patch
* `Chore`: A change to the build process or tooling
* `Test`: Unit / Integration test updates
* `Doc`: Documentation updates
* `Nit`: A small change related to style or snytax
* `Refactor`: A change to refactor code

**Allowed `<scope>`**

Scope is optional, but it should refer to a subsystem of the engine.

**`<subject>`**

Subject should contain a succinct description of the change.

* Use present tense, "change" not "changed" or "changes"
* Capitalize the first letter
* Don't end with a period

**`<message>`**

* Use present tense, as in `<subject>`
* Include whatever details and motivations are relevant to the change

**Examples**

```
Feat(Scene): Add glTF Camera Loading
Updated Scene loading to create Camera objects

Nit: Switch to for(auto& it) from for(int i)

Chore(Build): Change default compiler to GCC
```

