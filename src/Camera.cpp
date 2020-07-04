#include "Camera.hpp"

#include <App.hpp>
#include <Log.hpp>
#include <Utils.hpp>
#include <Window.hpp>

//Camera * Camera::_sInst = nullptr;

Camera::Camera()
{
    const glm::ivec2& size = glm::ivec2(App::Inst()->GetWindow()->GetWidth(),
                                        App::Inst()->GetWindow()->GetHeight());
    SetAspect(size);
    SetViewportSize(_mViewportSize);
}

glm::mat4 Camera::GetView() const
{
    return glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetForward(), _mUp);
}

glm::mat4 Camera::GetProjection() const
{
    if (_mMode == Mode::Perspective)
    {
        return glm::perspective(_mFovX, _mAspect, _mClip[0], _mClip[1]);
    }
    else if (_mMode == Mode::Orthographic)
    {
        const auto& view = GetViewport();
        return glm::ortho(view[0], view[1], view[2], view[3], _mClip[0], _mClip[1]);
    }

    return glm::mat4(1.0f);
}

void Camera::SetMode(Mode mode)
{
    _mMode = mode;
}

void Camera::SetAspect(float aspect)
{
    _mAspect = aspect;
}

void Camera::SetAspect(const glm::vec2& size)
{
    _mAspect = size.x / size.y;
}

void Camera::SetFOVX(float fovx)
{
    _mFovX = fovx;
}

void Camera::SetFOVY(float fovy)
{
    _mFovX = 2.0f * atanf(tanf(fovy * 0.5f) * _mAspect);
}

void Camera::SetViewportScale(float left, float right, float bottom, float top)
{
    _mViewportScale = glm::vec4(left, right, bottom, top);
}

void Camera::SetViewportScale(const glm::vec4& viewScale)
{
    _mViewportScale = viewScale;
}

void Camera::SetViewportSize(float width, float height)
{
    _mViewportSize.x = width;
    _mViewportSize.y = height;
}

void Camera::SetViewportSize(const glm::vec2& viewSize)
{
    _mViewportSize = viewSize;
}

glm::vec4 Camera::GetViewport() const
{
    glm::vec4 scale = GetViewportScale();
    glm::vec2 size = GetViewportSize();

    if (_mAspect > 1.0f)
    {
        size.y /= _mAspect;
    }
    else
    {
        size.x *= _mAspect;
    }

    return glm::vec4(
        size.x * scale[0],
        size.x * scale[1],
        size.y * scale[2],
        size.y * scale[3]);
}

void Camera::SetClip(float near, float far)
{
    _mClip.x = near;
    _mClip.y = far;
}

void Camera::SetClip(const glm::vec2& clip)
{
    _mClip = clip;
}

void Camera::SetUp(const glm::vec3& up)
{
    _mUp = up;
}

void Camera::SetForward(const glm::vec3& forward)
{
    if ((normalize(forward) + _mUp) == glm::vec3(0.0f))
    {
        SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }
    else
    {
        SetRotation(glm::quatLookAt(glm::normalize(forward), _mUp));
    }
}

glm::vec3 Camera::GetForward() const
{
    return glm::rotate(GetWorldRotation(), Utils::GetWorldForward());
}

void Camera::SetLookAt(const glm::vec3& point)
{
    SetForward(point - GetPosition());
}

void Camera::SetAutoResize(bool autoResize)
{
    _mAutoResize = autoResize;
}

void Camera::SetDirection(glm::vec3 dir)
{
    _mDirection = dir;
}

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

void Camera::HandleRotation(float xoffset, float yoffset)
{
    glm::vec2 delta(xoffset, yoffset);

    delta *= _mRotateSpeed;
    delta.x *= -1.0f;

    if (_mInverse)
        delta.y *= -1.0f;

    glm::vec3 forward = GetForward();
    glm::vec3 right = glm::normalize(glm::cross(Utils::GetWorldUp(), forward));
    SetUp(glm::cross(forward, right));

    glm::quat rotation = GetRotation();
    rotation = glm::angleAxis(delta.x, GetUp()) * rotation;
    rotation = glm::angleAxis(delta.y, GetRight()) * rotation;
    SetRotation(rotation);
}