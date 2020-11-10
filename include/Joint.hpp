#ifndef JOINT_HPP
#define JOINT_HPP

#include <vector>

#include <Math.hpp>

class Joint //: public GameObject
{
public:
	Joint() = default;
	Joint(int index, std::string name, glm::mat4 localBindTransform);
	~Joint() = default;

	void AddChild(Joint child);

	glm::mat4 GetTransform() { return _mTransform; }
	void SetTransform(glm::mat4 transform) { _mTransform = transform; }

	// This is called ONCE in setup and is called on the ROOT parent joint
	void CalcInverseBindTransform(glm::mat4 parentBindTransform);

	int GetIndex() { return _mIndex; }

	std::vector<Joint>& GetChildren() { return _mChildren; }

private:
	int _mIndex;
	std::string _mName;
	std::vector<Joint> _mChildren;

	// These transforms are in model space just like
	// normal transforms passed in so we can manipulate 
	// them in the shader.
	glm::mat4 _mTransform;

	// Original transform in model space, but inversed.
	glm::mat4 _mInverseBindTransform = glm::mat4();

};

#endif // JOINT_HPP
