Toon
===
[![CircleCI](https://circleci.com/gh/benjinx/Toon.svg?style=svg)](https://circleci.com/gh/benjinx/Toon)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/benjinx/Toon?svg=true)](https://ci.appveyor.com/project/benjinx/Toon)

A modular C++ Game Engine, designed to easily create Graphics demos. (Later support for Physics, Networking, and more).

Building
---

#### Linux && Windows

```
git clone https://github.com/benjinx/Toon.git
cd Toon
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

## Contributing

### Pull Requests

Use the "Branch and Pull Request" model for contributing changes.
