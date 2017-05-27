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
}

void Camera::Update(float dt)
{
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	_mViewMat = glm::lookAt(_mPosition, _mPosition + camFront, _mUp);
}

void Camera::Translate(glm::vec3 amount)
{
    // rotate amount to camera local then apply.
    glm::vec4 tmp    = glm::vec4(amount, 1.0f);
    glm::vec4 newAmt = tmp * _mViewMat;
    _mPosition += glm::vec3(newAmt.x, newAmt.y, newAmt.z);
    _mTarget += glm::vec3(newAmt.x, newAmt.y, newAmt.z);
    _mViewMat = glm::lookAt(_mPosition, _mTarget, _mUp);
}

glm::vec2 Camera::GetResolution()
{
	return glm::vec2((float)_mpWindow->GetWidth(), (float)_mpWindow->GetHeight());
}