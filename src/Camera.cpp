#include "Camera.h"

#include "Window.h"

void Camera::Init(Window* aWindow, glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up)
{
    _mpWindow  = aWindow;
    _mPosition = cameraPos;
    _mTarget   = cameraTarget;

    // 1 - FOV, 2 - Aspect Ratio, 3 - Near clipping, 4 - Far clipping
    _mProjectionMat =
        glm::perspective(45.0f, (float)_mpWindow->GetWidth() / (float)_mpWindow->GetHeight(), 0.1f, 10000.0f);

    // Camera Dir
    _mForward = glm::normalize(_mPosition - _mTarget);

    // Camera Right Axis
    _mRight = glm::normalize(glm::cross(up, _mForward));

    // Camera Up Axis
    _mUp = glm::cross(_mForward, _mRight);

    // Look At
    _mViewMat = glm::lookAt(_mPosition, _mTarget, up);
    _mWorldUp = up;

    // Rotate model
    // glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // View movement
    // glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f);
	_mVelocity = glm::vec3(0);
	_mAcceleration = glm::vec3(0);
	_mForce = glm::vec3(0);

	// Movement vals
	_mMovementSpeed = 2.0f;
	_mCamFront = glm::vec3(0.0f, 0.0f, -1.0f);
	_mFirstMouse = true;
}

void Camera::Update(float dt)
{
	//_mMovementSpeed = 2.0f * dt;
	// Drag
	//float dragCoeff = 5.0f, surfaceArea = 0.5f, fluidDensity = 1.293f; // 1.293f is airDensity

	//glm::vec3 dragForce = (dragCoeff * fluidDensity * surfaceArea * (_mVelocity * _mVelocity)) / 2.0f;
	//AddForce(dragForce);


	//UpdateFirstOrder(dt);

	std::cout << "Front " << _mCamFront.x << ", " << _mCamFront.y << ", " << _mCamFront.z << std::endl;

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

	switch (dir)
	{
	case FORWARD:
		_mPosition += velocity * _mCamFront;
		break;
	case BACKWARD:
		_mPosition -= velocity * _mCamFront;
		break;
	case LEFT:
		_mPosition -= glm::normalize(glm::cross(_mCamFront, _mUp)) * velocity;
		break;
	case RIGHT:
		_mPosition += glm::normalize(glm::cross(_mCamFront, _mUp)) * velocity;
		break;
	case UP:
		_mPosition += velocity * glm::normalize(glm::cross(_mForward, _mRight));
		break;
	case DOWN:
		_mPosition -= velocity * glm::normalize(glm::cross(_mForward, _mRight));
		break;
	default:
		break;
	}
}

void Camera::HandleRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_mYaw += xoffset;
	_mPitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_mPitch > 89.0f)
		_mPitch = -89.0f;
	if (_mPitch < -89.0f)
		_mPitch = 89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(_mYaw)) * cos(glm::radians(_mPitch));
	front.y = sin(glm::radians(_mPitch));
	front.z = sin(glm::radians(_mYaw)) * cos(glm::radians(_mPitch));
	_mCamFront = glm::normalize(front);
	_mRight = glm::normalize(glm::cross(_mCamFront, _mWorldUp));
	_mUp = glm::normalize(glm::cross(_mRight, _mCamFront));
}

glm::vec2 Camera::GetResolution()
{
	return glm::vec2((float)_mpWindow->GetWidth(), (float)_mpWindow->GetHeight());
}