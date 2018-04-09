#ifndef CAMERA_H
#define CAMERA_H

#include "Common.hpp"

enum Direction
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Window;

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    static Camera& instance()
    {
        static Camera* instance = new Camera();
        return *instance;
    }
    ~Camera(){};

    void Init(glm::vec3 cameraPos, glm::vec3 cameraTarget);

    void Update(float dt);

    glm::mat4 GetViewMat() { return _mViewMat; }
    glm::mat4 GetProjectionMat() { return _mProjectionMat; }
    glm::vec3 GetCameraPos() { return _mPosition; }
    void SetCameraPos(glm::vec3 position) { _mPosition = position; }
    glm::vec3 GetCameraVelocity() { return _mVelocity; }
    void SetCameraVelocity(glm::vec3 velocity) { _mVelocity = velocity; }
	glm::vec2 GetResolution();
	glm::vec3 GetCameraForward() { return _mForward; }

	void HandleRotation(float x, float y);
	void HandleMovement(Direction dir, float dt);
	void HandleFoV(float xoffset, float yoffset);
	void AddForce(glm::vec3 force);
	void UpdateAcceleration();
	void UpdateFirstOrder(float dt);

private:
    Camera(){};
    glm::mat4 _mViewMat, _mProjectionMat;
    glm::vec3 _mPosition,
			  _mVelocity,
			  _mAcceleration,
			  _mForce,
			  _mForward,
			  _mRight,
			  _mUp,
			  _mTarget,
			  _mWorldUp;
	float _mFoV;
	float _mYaw = -90.0f, _mPitch = 0.0f, _mRoll = 0.0f;
	float _mMovementSpeed;
	float _mMouseSensitivity;
	float _mZoom;
	glm::vec3 _mCamFront;
	bool			   _mFirstMouse;
	float			   _mLastX = 640, _mLastY = 360;

    // clip coords created via - V clip = M projection * M view * M model * V local
};

#endif // CAMERA_H
