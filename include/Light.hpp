#ifndef LIGHT_H
#define LIGHT_H

#include <Config.hpp>
#include <GameObject.hpp>
#include <Math.hpp>

class Light : public GameObject
{
public:
	Light() = default;
	virtual ~Light() = default;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 direction);

	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection();
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, float constant, float linear, float quadratic);

	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);

	float GetConstant() { return _mConstant; }
	float GetLinear() { return _mLinear; }
	float GetQuadratic() { return _mQuadratic; }

private:
	float _mConstant,
		  _mLinear,
		  _mQuadratic;
};

class SpotLight : public Light
{
public:
	SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, float outerCutoff);

	void SetDirection(glm::vec3 direction);
	void SetCutOff(float cutOff);
	void SetOuterCutOff(float outerCutOff);

	glm::vec3 GetDirection();
	float GetCutOff() { return _mCutOff; }
	float GetOuterCutOff() { return _mOuterCutOff; }

private:
	float _mCutOff,
		  _mOuterCutOff;
};

#endif // LIGHT_H