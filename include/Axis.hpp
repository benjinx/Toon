#ifndef AXIS_HPP
#define AXIS_HPP

#include <Config.hpp>
#include <Mesh.hpp>
#include <OpenGL.hpp>

#include <memory>

class Shader;

class Axis
{
public:
    Axis();

    void Render(glm::mat4 transform);
private:
    std::unique_ptr<Mesh> _mMesh = nullptr;

};

#endif // AXIS_HPP