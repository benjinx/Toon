#include "Rigidbody.h"
#include "GameObject.h"

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

void Rigidbody::UpdateFirstOrder(GameObject gameObject, const float dt)
{
	gameObject.SetPosition(gameObject.GetPosition() + _mVelocity * dt);
	_mVelocity += _mAcceleration * dt;

	UpdateAcceleration();
}

// clamp to ground (simple bounce): 
//	if object goes into ground, clamp it to the ground and 
//	use the remaining velocity for a bounce
void Rigidbody::ClampToGround(GameObject gameObject, const float groundHeight, const float restitution)
{
	if (gameObject.GetTransform().position.y < groundHeight)
	{
		glm::vec3 newPos = gameObject.GetPosition();
		newPos.y = groundHeight + (groundHeight - gameObject.GetPosition().y) * restitution;
		gameObject.SetPosition(newPos);
		_mVelocity.y = -_mVelocity.y * restitution;

		// Rv = V - 2dot(N, v) * N
		// N is surface normal
	}
}