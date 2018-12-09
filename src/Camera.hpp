#ifndef CAMERA_H
#define CAMERA_H

#include "Config.hpp"

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

class Camera
{
public:
    
	static Camera& Inst()
    {
		if (!_sInst) {
			_sInst = new Camera();
		}
        return *_sInst;
    }

	static void Delete() {
		if (_sInst) {
			delete _sInst;
			_sInst = nullptr;
		}
	}

    ~Camera() = default;

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

	static Camera * _sInst;

    Camera() = default;

    glm::mat4 _mViewMat, 
			  _mProjectionMat;
			  
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
	float _mYaw = -90.0f, 
	      _mPitch = 0.0f;
	float _mMovementSpeed = 2.5f;
	bool  _mFirstMouse;

};

#endif // CAMERA_H
