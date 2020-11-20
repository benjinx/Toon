#ifndef KEY_FRAME_HPP
#define KEY_FRAME_HPP

#include <unordered_map>
#include <Temporality/Math.hpp>

class KeyFrame
{
public:
	KeyFrame() = default;
	KeyFrame(float timestamp, std::unordered_map<std::string, glm::mat4> poses);
	~KeyFrame() = default;

	float GetTimestamp() { return _mTimestamp; }

	std::unordered_map<std::string, glm::mat4> GetKeyFramePoses() { return _mPoses; }

private:
	// Name/Transform
	// These transforms are in relation to the parents joint, meaning LOCAL space.
	// This is done to make it easier to interpolate between poses.
	std::unordered_map<std::string, glm::mat4> _mPoses;

	// Time in seconds from the start of the animation when this occurs
	float _mTimestamp;

};

#endif // KEY_FRAME_HPP