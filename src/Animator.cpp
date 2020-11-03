#include <Animator.hpp>

#include <Mesh.hpp>

Animator::Animator(std::shared_ptr<Mesh> mesh)
{
	_mMesh = mesh.get();
}

void Animator::DoAnimation(Animation animation)
{
	_mAnimationTime = 0;
	_mCurrentAnim = animation;
}

void Animator::Update(const float dt)
{
	// Need to check if the currentAnim is == null
	// #3 @ 5:35

}