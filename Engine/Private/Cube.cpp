#include <Toon/Cube.hpp>

namespace Toon {

Cube::Cube()
{

}

void Cube::Update()
{
    float dt = 1.0f;
    _mRigidbody.UpdateFirstOrder(this, dt);
}

} // namespace Toon