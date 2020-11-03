#include <Animation.hpp>

Animation::Animation(float length, std::vector<KeyFrame> keyFrames)
{
	_mLength = length;
	_mKeyFrames = keyFrames;
}