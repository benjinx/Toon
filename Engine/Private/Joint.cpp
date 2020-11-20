#include <Temporality/Joint.hpp>

Joint::Joint(int index, std::string name, glm::mat4 localBindTransform)
{
	_mIndex = index;
	_mName = name;
}

void Joint::AddChild(Joint child)
{
	_mChildren.push_back(child);
}

void Joint::CalcInverseBindTransform(glm::mat4 parentBindTransform)
{
	/*glm::mat4 bindTransform = parentBindTransform * _mLocalBindTransform;
	_mInverseBindTransform = glm::inverse(bindTransform);
	for (Joint& child : _mChildren)
	{
		child.CalcInverseBindTransform(bindTransform);
	}*/
}