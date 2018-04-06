#include "Camera.h"

#include "Window.h"
#include "Application.h"

void Camera::Init(glm::vec3 cameraPos, glm::vec3 cameraTarget)
{
    _mPosition = cameraPos;
    _mTarget   = cameraTarget;
	_mFoV = 45.0f;

    // 1 - FOV, 2 - Aspect Ratio, 3 - Near clipping, 4 - Far clipping
    _mProjectionMat =
        glm::perspective(glm::radians(_mFoV), (float)Application::Inst()->GetWindowWidth() / (float)Application::Inst()->GetWindowHeight(), 0.1f, 10000.0f);

    // Camera Dir
	_mForward = glm::normalize(_mPosition - _mTarget);

	// Set world up
	_mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera Right Axis
    _mRight = glm::normalize(glm::cross(_mWorldUp, _mForward));

    // Camera Up Axis
    _mUp = glm::cross(_mForward, _mRight);

    // Look At
    _mViewMat = glm::lookAt(_mPosition, _mTarget, _mWorldUp);

    // View movement
    // glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f);
	_mVelocity = glm::vec3(0);
	_mAcceleration = glm::vec3(0);
	_mForce = glm::vec3(0);

	// Movement vals
	_mMovementSpeed = 0.3f;
	_mCamFront = glm::vec3(0.0f, 0.0f, -1.0f);
	_mFirstMouse = true;
}

void Camera::Update(float dt)
{
	_mProjectionMat = glm::perspective(glm::radians(_mFoV), (float)Application::Inst()->GetWindowWidth() / (float)Application::Inst()->GetWindowHeight(), 0.1f, 10000.0f);
	_mViewMat = glm::lookAt(_mPosition, _mPosition + _mCamFront, _mUp);
}

void Camera::AddForce(glm::vec3 force)
{
	_mForce += force;
}

void Camera::UpdateAcceleration()
{
	//_mAcceleration = (_mForce * _mMassInv) + _mAccelerationFixed;
	_mAcceleration = _mForce; // Assuming mass for camera is 1, meaning massInv is the same, also no gravity applied to camera.
	_mForce = glm::vec3(0);
}

void Camera::UpdateFirstOrder(float dt)
{
	_mPosition += _mVelocity * dt;

	/*if (_mVelocity.x <= 2.0f && _mVelocity.x >= -2.0f &&
		_mVelocity.y <= 2.0f && _mVelocity.y >= -2.0f &&
		_mVelocity.z <= 2.0f && _mVelocity.z >= -2.0f)
	{*/
		_mVelocity += _mAcceleration * dt;
	//}
	//else
	//{
		// add velocity cap here.
	//}

	UpdateAcceleration();
}

void Camera::HandleMovement(Direction dir, float dt)
{
	float velocity = _mMovementSpeed * dt;
    printf("%f\n", _mMovementSpeed);

	switch (dir)
	{
	case FORWARD:
		_mPosition += _mCamFront * velocity;
		break;
	case BACKWARD:
		_mPosition -= _mCamFront * velocity;
		break;
	case LEFT:
		_mPosition -= glm::normalize(glm::cross(_mCamFront, _mUp)) * velocity;
		break;
	case RIGHT:
		_mPosition += glm::normalize(glm::cross(_mCamFront, _mUp)) * velocity;
		break;
	case UP:
		_mPosition += glm::normalize(glm::cross(_mCamFront, _mRight)) * velocity;
		break;
	case DOWN:
		_mPosition -= glm::normalize(glm::cross(_mCamFront, _mRight)) * velocity;
		break;
	default:
		break;
	}
}

void Camera::HandleRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_mYaw += xoffset;
	_mPitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_mPitch > 89.0f)
		_mPitch = 89.0f;
	if (_mPitch < -89.0f)
		_mPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(_mPitch)) * cos(glm::radians(_mYaw));
	front.y = sin(glm::radians(_mPitch));
	front.z = cos(glm::radians(_mPitch)) * sin(glm::radians(_mYaw));
	_mCamFront = glm::normalize(front);
	_mRight = glm::normalize(glm::cross(_mCamFront, _mWorldUp));
	_mUp = glm::normalize(glm::cross(_mRight, _mCamFront));
}

void Camera::HandleFoV(float xoffset, float yoffset)
{
	if (_mFoV >= 1.0f && _mFoV <= 45.0f)
		_mFoV -= yoffset;
	if (_mFoV <= 1.0f)
		_mFoV = 1.0f;
	if (_mFoV >= 45.0f)
		_mFoV = 45.0f;
}

glm::vec2 Camera::GetResolution()
{
	return glm::vec2((float)Application::Inst()->GetWindowWidth(), (float)Application::Inst()->GetWindowHeight());
}
