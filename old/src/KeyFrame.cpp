#include <KeyFrame.hpp>

KeyFrame::KeyFrame(float timestamp, std::unordered_map<std::string, glm::mat4> poses)
{
	_mTimestamp = timestamp;
	_mPoses = poses;
}
