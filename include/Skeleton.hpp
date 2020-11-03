#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <vector>

#include <Joint.hpp>

class Skeleton
{
public:
	Skeleton() = default;
	Skeleton(std::vector<Joint> joints);
	~Skeleton() = default;

	void SetName(std::string name) { _mName = name; }
	std::string GetName() { return _mName; }

	void SetInverseBindMatrices(int inverseBindMatrices) { _mInverseBindMatrices = inverseBindMatrices; }
	int GetInverseBindMatrices() { return _mInverseBindMatrices; }

	void SetJoints(std::vector<Joint> joints) { _mJoints = joints; }
	std::vector<Joint> GetJoints() { return _mJoints; }

private:
	// The list of joints
	std::vector<Joint> _mJoints;

	std::string _mName;

	int _mInverseBindMatrices;
};

#endif // SKELETON_HPP