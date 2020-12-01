# Temporality Code Style Guidelines

## Indentation

The indent size is 4 spaces. Tabs should never be used.

*Right*
```
int func() {
    return 0;
}
```

*Wrong*
```
int func() {
  return 0;
}
int func() {
        return 0;
}
```

Namespaces should have no indent.

*Right*
```
namespace foo {

namespace bar {

void func() {

}

} // namespace bar

} // namespace foo
```

*Wrong*
```
namespace foo {
    namespace bar {
        void func() {

        }
    }
}
```

case labels should line up with their switch statements. The case statements are indented.

*Right*
```
switch (condition) {
case 0:
case 1:
    ++i;
    break;
case 2:
    --i;
    break;
default:
    i = 0;
}
```

*Wrong*
```
switch (condition) {
    case 0:
    case 1:
        ++i;
        break;
    case 2:
        --i;
    break;
    default:
}
```

Boolean expressions that span multiple lines should have their operators on the left side.

*Right*
```
return checkFirstCase()
    || checkSecondCase()
    || (checkThirdCase() && checkEdgeCase());
```

*Wrong*
```
return checkFirstCase() ||
    checkSecondCase() ||
    (checkThirdCase() && checkEdgeCase());
```

## Spacing

Do not put spaces around unary operators.

*Right*
```
++i;
int* j = &i;
```

*Wrong*
```
++ i;
int* j = & i;
```

Do put spaces around binary and ternary operators.

*Right*
```
y = m * x + b;
f(a, b);
c = a | b;
return (condition ? 1 : 0);
```

*Wrong*
```
y = m*x+b;
f(a,b);
c = a|b;
return(condition?1:0);
```

Do put spaces around the colon in a range-based for loop.

*Right*
```
vector<glm::vec2> sizes;
for (auto& size : sizes) {
    Log("%s", glm::to_string(size));
}
```

*Wrong*
```
vector<glm::vec2> sizes;
for (auto& size: sizes) {
    Log("%s", glm::to_string(size));
}
```

Do not put spaces before commas or semicolons.

*Right*
```
for (int i = 0; i < 10; ++i){
    doSomething();
}

f(a, b);
```

*Wrong*
```
for (int i = 0 ; i < 10 ; ++i)
    doSomething();

f(a , b) ;
```

Do put spaces between control statements and parenthesis.

*Right*
```
if (condition) {
    doSomething();
}
```

*Wrong*
```
if(condition) {
    doSomething();
}
```

when brace-initalizing an object, do put spaces between the object name, and the concents of the braces.

*Right*
```
glm::vec2 size { 0.0f, 10.0f };
```

*Wrong*
```
glm::vec2 size{ 0.0f, 10.0f };
glm::vec2 size {0.0f, 10.0f};
```

## Line Breaking

Every statement gets its own line.

*Right*
```
++x;
++y;
if (condition) {
    doSomething();
}
```

*Wrong*
```
++x; ++y;
if (condition) { doSomething(); }
```

An else should go on the same line as the previous end-bracket.

*Right*
```
if (condition) {
    doSomething();
} else if (otherCondition) {
    doSomethingElse();
} else {
    doNothing();
}
```

*Wrong*
```
if (condition) {
    doSomething();
}
else if (otherCondition) {
    doSomethingElse();
}
else {
    doNothing();
}
```

Namespaces should have one line of padding after opening and before closing.

*Right*
```
namespace foo {

namespace bar {

void doSomething();

} // namespace bar
} // namespace foo
```

*Wrong*
```
namespace foo {
namespace bar {
void doSomething();
} // namespace bar
} // namespace foo
```

## Braces

Functions, and Classes should have their braces on their own line.

*Right*
```
int main()
{
    return 0;
}

class Test
{

}; // class Test
```

*Wrong*
```
int main() {
    return 0;
}

class Test {

}; // class Test
```

Namespaces should have braces on the same line.

*Right*
```
namespace foo {

} // namespace foo
```

*Wrong*
```
namespace foo
{

} // namespace foo
```

All control statements must use braces.

*Right*
```
if (condition) {
    doSomething();
} else {
    doSomethingElse();
}

while (condition) {
    doSomething();
}
```

*Wrong*
```
if (condition)
    doSomething();
else
    doSomethingElse();

if (condition)
    return;

while (condition) doSomething();
```

Empty control statements must use empty braces.

*Right*
```
for (; wait < 100; wait++) { }
while (condition) { }
```

*Wrong*
```
for (; wait < 100; wait++);
while (condition);
```

## Null, false, and zero

In C++, null should be represented by `nullptr`. In C, null should be represented by `NULL`.

In C++ and C, `bool` values should be represented by `true` or `false`.

In library-specific code, you shuold use the boolean defines for that library, e.g. `GL_TRUE`, `SDL_FALSE`

Tests for true/false, null/non-null, and zero/non-zero should all be done without equality expressions.

*Right*
```
if (condition) {

}

if (!ptr) {

}

if (!count) {

}
```

*Wrong*
```
if (condition == true) {

}

if (ptr == NULL) {

}

if (count == 0) {

}
```

## Floating Point Literals

Always append `f` or `.0f` to floating point literals. Do not append `.f`.

*Right*
```
const float angle = glm::radians(90.0f);

const float pi = 3.14159f;
```

*Wrong*
```
const float angle = glm::radians(90.f);

const float pi = 3.14159;
```

## Names

Use CamelCase for classes, and structs. Capitalize all letters of an acronym as appropriate.

*Right*
```
struct Data;
class JSONElement;
class glTF2;
```

*Wrong*
```
struct data;
class jsonElement;
class GLTF2;
```

Use snakeCase for namespaces. Capitalize all letters of an acronym as appropriate.

*Right*
```
namespace foo {

namespace glTF2 {

} // namespace glTF2
} // namespace foo
```

*Wrong*
```
namespace Foo {

namespace gltf2 {

} // namespace gltf2
} // namespace Foo
```

Use full words, except for common abbreviations and iterators.

*Right*
```
size_t length;
MeshComponent* meshComponent = new MeshComponent();
for (int i = 0; i < 10; ++i) { }
void parseGameobject();
```

*Wrong*
```
size_t len, l;
MeshComponent* comp = new MeshComponent();
void pGameobject();
void parseG();
```

Private member variables should be prefixed with `_m`. Private static member variables should be prefixed with `_s`. Both member variables should use snakeCase.

*Right*
```
class Test
{
public:

    ...

private:

    static int _sCount;

    int _mCount;
}
```

*Wrong*
```
class Test
{
public:

    ...

private:

    static int _count;
    static int Count;
    static int _Count;
    static int s_Count;

    int count;
    int Count;
    int mCount;
    int _Count;
    int m_count;
}
```

Prefix boolean variables with past- or current-tense verb.

*Right*
```
bool isValid;
bool didSendData;
```

*Wrong*
```
bool valid;
bool sendData;
```

Prefix setters with `Set` and getters with `Get`.

*Right*
```
void SetPosition(const glm::vec3& position);
glm::mat4 GetTransform() const;
```

*Wrong*
```
void Position(const glm::vec3& position);
glm::mat4 Transform() const;
```

Use descriptive verbs in function names.

*Right*
```
std::uint8_t* readDataFromFile();
bool convertToJSON();
```

*Wrong*
```
std::uint8_t* getData();
bool toJSON();
```

### Comments

Classes and Namespaces should end with a comment `// class name` or `// namespace name`.

*Right*
```
namespace foo {

namespace bar {

class Test
{

}; // class Test

} // namespace bar

} // namespace foo
```

*Wrong*
```
namespace foo {

namespace bar {

class Test
{

};

}

}
```

Prefer enums to booleans as parameters, except for setter functions.

*Right*
```
loadFromFile("test.txt", AllowNotFound);
doSomething(something, AllowFooBar);
setResizable(false);
```

*Wrong*
```
loadFromFile("test.txt", true);
doSomething(something, false);
setResizable(NotResizable);
```

Prefer inline functions to macros.

Enum members should be CamelCase.

Prefer const to #define.

`#define`'d constants should be ALL_CAPS.

Macros that expand like functions should be named in CamelCase.

*Right*
```
#define LogInfo(M) PrintLog(M, __LINE__, __FILE__);
```

*Wrong*
```
#define LOG_INFO(M) PrintLog(M, __LINE__, __FILE__);
```

## Classes

All non-trivial classes should disable the copy constructor and assignmenet operator, except when desired. Use the macro `DISALLOW_COPY_AND_ASSIGN` from `Config.hpp.`

```
class Test
{
public:
    DISALLOW_COPY_AND_ASSIGN(Test);
}; // class Test
```

Use explicit to stop unwanted type conversions.

*Right*
```
class Box
{
public:

    Box(glm::vec4);
}; // class Box
```

*Wrong*
```
```

Constructors should initialize all members either in their declarations, or with initializer syntax. One per line, preceeded by `:` or `,` depending.

*Right*
```
class Test
{
public:

    Test(const std::string& name, const glm::vec2& size)
        : _mName(name)
        , _mSize(size)
    { }

private:

    int _mCount = 0;

    std::string _mName;

    glm::vec2 _mSize;
}; // class Test
```

*Wrong*
```
class Test
{
public:

    Test(const std::string& name, const glm::vec2& size) : _mName(name)
    {
        _mCount = 0;
        _mSize = size;
    }

private:

    int _mCount;

    std::string _mName;

    glm::vec2 _mSize;
}
```

## Best Practices

Use range-based for when index is not needed. Use auto for iterators.

*Right*
```
std::vector<std::unique_ptr<Entity>> entitys;
for (auto& entity : entitys) {
    entity->Update(dt);
}

// Index is used
for (size_t i = 0; i < entitys.size(); ++i) {
    LogInfo("Updating Gameobject #%zu", i);
}
```

*Wrong*
```
std::vector<std::unique_ptr<Entity>> entitys;

std::vector<std::unique_ptr<Entity>> it = entitys.begin();
for (; it != entitys.end(); ++it) {
    entity->Update(dt);
}

// Index is not used
for (size_t i = 0; i < entitys.size(); ++i) {
    entitys[i]->Update(dt);
}
```

All pointers should be written such that the * is NOT spaced from the type, but is spaced from the name.

*Right*
```
int* x;
std::vector<Entitys*> entitys;
```

*Wrong*
```
int * x;
int *y;
std::vector<Entitys *> entitys;
```

`#include` statements should be grouped into the following categories: Temporality, System, and Third Party. Within those categories they should be alphabetized, except when order is important.

*Right*
```
#include <Config.hpp>
#include <Entity.hpp>
#include <Mesh.hpp>

#include <memory>
#include <vector>

#include <tiny_gltf.h>
```

*Wrong*
```
#include <Entity.hpp>
#include <Mesh.hpp>
#include <vector>
#include <memory>
#include <Config.hpp>
```

Using statements are only allowed inside functions or source files. Do not pollute the global namespace.

*Right*
```
void doTimeStuff() {
    using std::chrono;
    ...
}
```

*Wrong*
```
using namespace std::chrono;

void doTimeStuff() {
    ...
}
```
Omit unnecessary parameter names from funtion declarions where the size or function name allow you to infer the use.

*Right*
```
void SetWidth(int);
void AddEntity(std::unique_ptr<Entity>&&);

void doSomething(int amountToAdd);
```

*Wrong*
```
void SetWidth(int width);
void AddEntity(std::unique_ptr<Entity>&& entity);

void doSomething(int);
```

Use `unsigned` instead of `unsigned int`.

Use `uint8_t` over `unsigned char*` for binary data.

Avoid `using namespace x`, instead prefix all types with `x::`. (e.g. `std::`, `glm::`).

Use `::` to explicitly request the global scope when referencing C functions with conflicting names:
```
void Socket::send() {
    ::send(_mSocket, _mBuffer.data(), _mBuffer.size(), 0);
}
```

All header files should use `#ifndef` guards named `FILENAME_HPP`, where FILENAME is an all-caps representation of the filename.

*Right*
```
```

*Wrong*
```
```
