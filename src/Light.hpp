#ifndef LIGHT_H
#define LIGHT_H

#include "Common.hpp"

class Light
{
public:
	Light();
	~Light();

	void SetPosition(glm::vec4 pos) { _mPosition = pos; }
	glm::vec4 GetPosition() { return _mPosition; }

private:
	glm::vec4 _mPosition; // No longer necessery when using directional lights.
};


// Directional Light
struct DirectionalLight : public Light
{
public:
	void SetDirection(glm::vec4 dir) { _mDirection = dir; }
	glm::vec4 GetDirection() { return _mDirection; }

private:
	glm::vec4 _mDirection;
};


// Point Light
struct PointLight : public Light
{
public:
	void SetConstant(float con) { _mConstant = con; }
	void SetLinear(float lin) { _mLinear = lin; }
	void SetQuadratic(float quad) { _mQuadratic = quad; }

	float GetConstant() { return _mConstant; }
	float GetLinear() { return _mLinear; }
	float GetQuadratic() { return _mQuadratic; }

private:
	// Point Light
	float _mConstant;
	float _mLinear;
	float _mQuadratic;
};


// Spot Light
struct SpotLight : public Light
{
public:
	void SetCutOff(float co) { _mCutOff = co; }
	float GetCutOff() { return _mCutOff; }

private:
	// Spot Light
	// also uses direction and position
	float _mCutOff;
};

#endif // LIGHT_H