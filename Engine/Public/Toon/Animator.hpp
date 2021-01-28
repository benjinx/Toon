#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <Toon/Animation.hpp>

#include <memory>

class Mesh;

class Animator
{
public:
	Animator() = default;
	Animator(std::shared_ptr<Mesh> mesh);
	~Animator() = default;

	void DoAnimation(Animation animation);

	void Update(const float dt);

private:
	// Need to have access to the mesh
	Mesh* _mMesh;

	Animation _mCurrentAnim;
	float _mAnimationTime;
};

#endif // ANIMATOR_HPP