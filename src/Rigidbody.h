#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "glm/glm.hpp"

class GameObject;

class Rigidbody
{
public:
	void AddForce(const glm::vec3 force);
	void SetMass(const float mass);
	void UpdateAcceleration();
	void UpdateFirstOrder(GameObject gameObject, const float dt);
	void ClampToGround(GameObject gameObject, const float groundHeight, const float restitution);

private:
	// transform
	
	// movement
	glm::vec3 _mVelocity, _mAccelerationFixed;

	// newton's stuff
	glm::vec3 _mAcceleration, _mForce;
	float _mMass, _mMassInv;
};

#endif // !RIGIDBODY_H
