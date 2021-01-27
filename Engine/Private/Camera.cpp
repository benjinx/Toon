#include "Temporality/Camera.hpp"

//#include <Temporality/App.hpp>
#include <Temporality/Log.hpp>
#include <Temporality/Utils.hpp>
//#include <Temporality/Window.hpp>

//Camera * Camera::_sInst = nullptr;

namespace Temporality {

TEMPORALITY_ENGINE_API
Camera::Camera()
{
    /*const glm::ivec2& size = glm::ivec2(App::Inst()->GetWindow()->GetWidth(),
                                        App::Inst()->GetWindow()->GetHeight());
    SetAspect(size);
    SetViewportSize(_mViewportSize);*/
}

TEMPORALITY_ENGINE_API
glm::mat4 Camera::GetView() const
{
    return glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetForward(), GetUp());
}

TEMPORALITY_ENGINE_API
glm::mat4 Camera::GetProjection() const
{
    if (_mMode == CameraMode::Perspective)
    {
        return glm::perspective(_mFovX, _mAspect, _mClip[0], _mClip[1]);
    }
    else if (_mMode == CameraMode::Orthographic)
    {
        const auto& view = GetViewport();
        return glm::ortho(view[0], view[1], view[2], view[3], _mClip[0], _mClip[1]);
    }

    return glm::mat4(1.0f);
}

TEMPORALITY_ENGINE_API
void Camera::SetMode(CameraMode mode)
{
    _mMode = mode;
}

TEMPORALITY_ENGINE_API
void Camera::SetAspect(float aspect)
{
    _mAspect = aspect;
}

TEMPORALITY_ENGINE_API
void Camera::SetAspect(const glm::vec2& size)
{
    _mAspect = size.x / size.y;
}

TEMPORALITY_ENGINE_API
void Camera::SetFOVX(float fovx)
{
    _mFovX = fovx;
}

TEMPORALITY_ENGINE_API
void Camera::SetFOVY(float fovy)
{
    _mFovX = 2.0f * atanf(tanf(fovy * 0.5f) * _mAspect);
}

TEMPORALITY_ENGINE_API
void Camera::SetViewportScale(const glm::vec4& viewScale)
{
    _mViewportScale = viewScale;
}

TEMPORALITY_ENGINE_API
void Camera::SetViewportSize(const glm::vec2& viewSize)
{
    _mViewportSize = viewSize;
}

TEMPORALITY_ENGINE_API
glm::vec4 Camera::GetViewport() const
{
    glm::vec4 scale = GetViewportScale();
    glm::vec2 size = GetViewportSize();

    if (_mAspect > 1.0f) {
        size.y /= _mAspect;
    }
    else {
        size.x *= _mAspect;
    }

    return glm::vec4(
        size.x * scale[0],
        size.x * scale[1],
        size.y * scale[2],
        size.y * scale[3]);
}

TEMPORALITY_ENGINE_API
void Camera::SetClip(const glm::vec2& clip)
{
    _mClip = clip;
}

TEMPORALITY_ENGINE_API
void Camera::SetUp(const glm::vec3& up)
{
    _mUp = up;
}

TEMPORALITY_ENGINE_API
void Camera::SetForward(const glm::vec3& forward)
{
    if ((normalize(forward) + _mUp) == glm::vec3(0.0f))
    {
        SetOrientation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }
    else
    {
        SetOrientation(glm::quatLookAt(glm::normalize(forward), _mUp));
    }
}

TEMPORALITY_ENGINE_API
glm::vec3 Camera::GetForward() const
{
    return glm::rotate(GetWorldOrientation(), GetWorldForward());
}

TEMPORALITY_ENGINE_API
glm::vec3 Camera::GetRight() const
{
    return glm::normalize(glm::cross(GetForward(), GetUp()));
}

TEMPORALITY_ENGINE_API
void Camera::SetLookAt(const glm::vec3& point)
{
    SetForward(point - GetPosition());
}

TEMPORALITY_ENGINE_API
void Camera::SetDirection(glm::vec3 dir)
{
    _mDirection = dir;
}

TEMPORALITY_ENGINE_API
void Camera::HandleMovement(float dt)
{
    float velocity = _mMovementSpeed * dt;
    
    glm::vec3 right = glm::normalize(glm::cross(_mUp, GetForward()));

    auto pos = GetPosition();

    pos += glm::normalize(glm::cross(GetForward(), _mUp)) * velocity * _mDirection.x;
    pos += glm::normalize(glm::cross(GetForward(), right)) * velocity * _mDirection.y;
    pos += GetForward() * velocity * _mDirection.z;

    SetPosition(pos);
}

TEMPORALITY_ENGINE_API
void Camera::HandleRotation(float deltaX, float deltaY)
{
    glm::vec2 delta(deltaX, deltaY);

    delta *= _mRotateSpeed;
    delta.x *= -1.0f;

    if (_mInverse)
        delta.y *= -1.0f;

    glm::vec3 forward = GetForward();
    glm::vec3 right = glm::normalize(glm::cross(GetWorldUp(), forward));
    SetUp(glm::cross(forward, right));

    glm::quat rotation = GetOrientation();
    rotation = glm::angleAxis(delta.x, GetUp()) * rotation;
    rotation = glm::angleAxis(delta.y, GetRight()) * rotation;
    SetOrientation(rotation);
}

} // namespace Temporality