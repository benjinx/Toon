#ifndef CUBE_H
#define CUBE_H

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Rigidbody.hpp>

namespace Toon {

class Cube : public Entity
{
public:
    Cube();
    virtual ~Cube() = default;

    void Update();

private:
    Rigidbody _mRigidbody;
};

} // namespace Toon

#endif // !CUBE_H
