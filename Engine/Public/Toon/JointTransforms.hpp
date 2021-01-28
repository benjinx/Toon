#ifndef JOINT_TRANSFORMS_HPP
#define JOINT_TRANSFORMS_HPP

#include <Toon/Math.hpp>

class JointTransforms
{
public:
	JointTransforms();
	~JointTransforms();

private:
	glm::vec3 _position;
	glm::quat _orientation;
};

#endif // JOINT_TRANSFORMS_HPP
