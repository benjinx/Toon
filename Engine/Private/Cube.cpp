#include <Temporality/Cube.hpp>

namespace Temporality {

Cube::Cube()
{

}

void Cube::Update()
{
    float dt = 1.0f;
    _mRigidbody.UpdateFirstOrder(this, dt);
}

} // namespace Temporality