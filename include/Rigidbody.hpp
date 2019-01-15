#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Config.hpp>
#include <Math.hpp>

class GameObject;

class Rigidbody
{
public:
	void AddForce(const glm::vec3 force);
	void SetMass(const float mass);
	void UpdateAcceleration();
	void UpdateFirstOrder(GameObject* gameObject, const float dt);
	void ClampToGround(GameObject* gameObject, const float groundHeight, const float restitution);

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

#endif // !RIGIDBODY_H
