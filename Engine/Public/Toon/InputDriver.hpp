#ifndef TOON_INPUT_DRIVER_HPP
#define TOON_INPUT_DRIVER_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/Keyboard.hpp>
#include <Toon/Mouse.hpp>
#include <Toon/Controller.hpp>

#include <memory>

namespace Toon {

// class MouseMovedEventData : public EventData
// {
// public:

//     ivec2 Position;
//     ivec2 Delta;

//     PyObject * PySerialize() const override {
//         PyObject * dict = PyDict_New();

//         PyObject * pos = PyTuple_New(2);
//         PyTuple_SET_ITEM(pos, 0, PyLong_FromLong(Position.x));
//         PyTuple_SET_ITEM(pos, 1, PyLong_FromLong(Position.y));
//         PyDict_SetItemString(dict, "Position", pos);

//         PyObject * delta = PyTuple_New(2);
//         PyTuple_SET_ITEM(delta, 0, PyLong_FromLong(Delta.x));
//         PyTuple_SET_ITEM(delta, 1, PyLong_FromLong(Delta.y));
//         PyDict_SetItemString(dict, "Delta", delta);

//         return dict;
//     }

// }; // class MouseMovedEventData

class TOON_ENGINE_API InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(InputDriver)

    InputDriver() = default;

    virtual ~InputDriver() = default;

    virtual glm::ivec2 GetMouseCoordinates() const = 0;

}; // class InputDriver

TOON_ENGINE_API
void SetInputDriver(std::unique_ptr<InputDriver> && driver);

TOON_ENGINE_API
InputDriver * GetInputDriver();

} // namespace Toon

#endif // TOON_INPUT_DRIVER_HPP
