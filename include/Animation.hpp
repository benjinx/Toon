#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include <KeyFrame.hpp>

class Animation
{
public:
	Animation();
	~Animation();

private:
	std::vector<KeyFrame> _mFrames;
};

#endif // ANIMATION_HPP