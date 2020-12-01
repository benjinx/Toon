#ifndef CUBE_H
#define CUBE_H

#include <Temporality/Config.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/Rigidbody.hpp>

namespace Temporality {

class Cube : public Entity
{
public:
    Cube();
    virtual ~Cube() = default;

    void Update();

private:
    Rigidbody _mRigidbody;
};

} // namespace Temporality

#endif // !CUBE_H
