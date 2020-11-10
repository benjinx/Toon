#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <vector>
#include <memory>

#include "Joint.hpp"

class Skeleton
{
public:
	Skeleton() = default;
	virtual ~Skeleton() = default;

	void SetName(std::string name) { _mName = name; }
	std::string GetName() { return _mName; }

	void SetInverseBindMatrices(int inverseBindMatrices) { _mInverseBindMatrices = inverseBindMatrices; }
	int GetInverseBindMatrices() { return _mInverseBindMatrices; }

	std::vector<std::unique_ptr<Joint>> & GetJoints() { return _mJoints; }

	void AddJoint(std::unique_ptr<Joint>&& joint) { _mJoints.push_back(std::move(joint)); }

private:
	// The list of joints
	std::vector<std::unique_ptr<Joint>> _mJoints;

	std::string _mName;

	int _mInverseBindMatrices;
};

#endif // SKELETON_HPP