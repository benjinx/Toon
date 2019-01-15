#include "Light.hpp"

#include <Utils.hpp>

DirectionalLight::DirectionalLight(glm::vec3 direction)
{
	SetDirection(direction);
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	if ((normalize(direction) + Utils::GetWorldUp()) == glm::vec3(0.0f)) {
		SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else {
		SetRotation(glm::quatLookAt(glm::normalize(direction), Utils::GetWorldUp()));
	}
}

glm::vec3 DirectionalLight::GetDirection()
{
	return glm::rotate(GetWorldRotation(), Utils::GetWorldForward());
}


PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic)
	: _mConstant(constant)
	, _mLinear(linear)
	, _mQuadratic(quadratic)
{
	SetPosition(position);
}

void PointLight::SetConstant(float constant)
{
	_mConstant = constant;
}

void PointLight::SetLinear(float linear)
{
	_mLinear = linear;
}

void PointLight::SetQuadratic(float quadratic)
{
	_mQuadratic = quadratic;
}


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff)
	: _mCutOff(cutOff)
	, _mOuterCutOff(outerCutOff)
{
	SetPosition(position);
	SetDirection(direction);
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	if ((normalize(direction) + Utils::GetWorldUp()) == glm::vec3(0.0f)) {
		SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else {
		SetRotation(glm::quatLookAt(glm::normalize(direction), Utils::GetWorldUp()));
	}
}

glm::vec3 SpotLight::GetDirection()
{
	return glm::rotate(GetWorldRotation(), Utils::GetWorldForward());
}

void SpotLight::SetCutOff(float cutOff)
{
	_mCutOff = cutOff;
}

void SpotLight::SetOuterCutOff(float outerCutOff)
{
	_mOuterCutOff = outerCutOff;
}