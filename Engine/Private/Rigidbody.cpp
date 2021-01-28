#include "Toon/Rigidbody.hpp"
#include "Toon/Entity.hpp"
#include <iostream>

namespace Toon {

void Rigidbody::AddForce(const glm::vec3 force)
{
    _mForce += force;
}

void Rigidbody::SetMass(const float mass)
{
    if (mass > 0.0f)
    {
        _mMassInv = 1.0f / (_mMass = mass);
    }
    else
    {
        _mMassInv = (_mMass = 0.0f);
    }
}

void Rigidbody::UpdateAcceleration()
{
    // F = ma --> a = F/m --> a = (F * mInv)
    _mAcceleration = (_mForce * _mMassInv) + _mAccelerationFixed;
    _mForce = glm::vec3(0);
}

void Rigidbody::UpdateFirstOrder(Entity* entity, const float dt)
{
    //if (_mVelocity.x > 0.01f || _mVelocity.y > 0.01f || _mVelocity.z > 0.01f)
    //{
        entity->SetPosition(entity->GetPosition() + (_mVelocity * dt));
        _mVelocity += _mAcceleration * dt;

        UpdateAcceleration();
    //}


    std::cout << "pos: " << entity->GetPosition().x << " " << entity->GetPosition().y << " " << entity->GetPosition().z << std::endl;
    std::cout << "dt: " << dt << std::endl;
    std::cout << "acc: " << _mAcceleration.x << " " << _mAcceleration.y << " " << _mAcceleration.z << std::endl;
    std::cout << "vel: " << _mVelocity.x << " " << _mVelocity.y << " " << _mVelocity.z << std::endl;
}

// clamp to ground (simple bounce): 
//    if object goes into ground, clamp it to the ground and 
//    use the remaining velocity for a bounce
void Rigidbody::ClampToGround(Entity* entity, const float groundHeight, const float restitution)
{
    if (entity->GetPosition().y < groundHeight)
    {
        if (!(entity->GetPosition().y > 0.001f))
        {
            glm::vec3 newPos = entity->GetPosition();
            newPos.y = groundHeight + (groundHeight - entity->GetPosition().y);
            entity->SetPosition(newPos);
            _mVelocity.y = (-_mVelocity.y) * restitution;
        }
        else
            entity->SetPosition(glm::vec3(entity->GetPosition().x, entity->GetPosition().y * 0, entity->GetPosition().z));

        // Rv = V - 2dot(N, v) * N
        // N is surface normal
    }
}

} // namespace Toon