#ifndef KEY_FRAME_HPP
#define KEY_FRAME_HPP

#include <vector>

#include <Joint.hpp>

class KeyFrame
{
public:
	KeyFrame();
	~KeyFrame();

private:
	// These transforms are in relation to the parents joint, meaning LOCAL space.
	// This is done to make it easier to interpolate between poses. 
	std::vector<glm::mat4> _mTransforms;
	float _mTimeStamp;
};

#endif // KEY_FRAME_HPP