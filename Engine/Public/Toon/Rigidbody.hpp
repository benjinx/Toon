#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

namespace Toon {

class Entity;

class Rigidbody
{
public:
    void AddForce(const glm::vec3 force);
    void SetMass(const float mass);
    void UpdateAcceleration();
    void UpdateFirstOrder(Entity* entity, const float dt);
    void ClampToGround(Entity* entity, const float groundHeight, const float restitution);

    void SetFixedAcceleration(glm::vec3 fixedAcceleration) { _mAccelerationFixed = fixedAcceleration; }
    void SetVelocity(glm::vec3 velocity) { _mVelocity = velocity; }
    void SetAcceleration(glm::vec3 acceleration) { _mAcceleration = acceleration; }
private:
    // transform
    
    // movement
    glm::vec3 _mVelocity, _mAccelerationFixed;

    // newton's stuff
    glm::vec3 _mAcceleration, _mForce;
    float _mMass, _mMassInv;
};

} // namespace Toon

#endif // !RIGIDBODY_H
