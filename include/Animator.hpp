#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <Animation.hpp>

class Animator
{
public:
	Animator();
	~Animator();

private:
	Animation _mCurrentAnim;
	float _mAnimationTime;
};

#endif // ANIMATOR_HPP