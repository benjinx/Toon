#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include <Toon/KeyFrame.hpp>

class Animation
{
public:
	Animation() = default;
	Animation(float length, std::vector<KeyFrame> keyFrames);
	~Animation() = default;

	float GetLength() { return _mLength; }
	void SetLength(float length) { _mLength = length; }

	std::vector<KeyFrame> GetKeyFrames() { return _mKeyFrames; }
	void SetKeyFrames(std::vector<KeyFrame> keyFrames) { _mKeyFrames = keyFrames; }

private:
	std::vector<KeyFrame> _mKeyFrames;
	float _mLength; // This is in seconds
};

#endif // ANIMATION_HPP