#ifndef JOINT_TRANSFORMS_HPP
#define JOINT_TRANSFORMS_HPP

#include <Temporality/Math.hpp>

class JointTransforms
{
public:
	JointTransforms();
	~JointTransforms();

private:
	glm::vec3 _mPosition;
	glm::quat _mRotation;
};

#endif // JOINT_TRANSFORMS_HPP
