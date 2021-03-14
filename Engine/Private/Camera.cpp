#include <Toon/Camera.hpp>

#include <Toon/GraphicsDriver.hpp>

namespace Toon {

TOON_ENGINE_API
Camera::Camera()
{
    auto gfx = GetGraphicsDriver();

    const glm::ivec2& size = gfx->GetWindowSize();
    SetAspect(size);
    SetViewportSize(size);

    /*_windowResizedEventHandlerID = gfx->WindowResizedEvent.AddListener(
        [=](const WindowResizedEventData * data) {
            SetAspect(data->Size);
            SetViewportSize(data->Size);
        }
    );*/
}

TOON_ENGINE_API
Camera::~Camera()
{
    auto gfx = GetGraphicsDriver();
    //gfx->WindowResizedEvent.RemoveListener(_windowResizedEventHandlerID);
}

TOON_ENGINE_API
glm::mat4 Camera::GetView() const
{
    return glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetForward(), GetUp());
}

TOON_ENGINE_API
glm::mat4 Camera::GetProjection() const
{
    if (_mode == CameraMode::Perspective)
    {
        return glm::perspective(_fovX, _aspect, _clip[0], _clip[1]);
    }
    else if (_mode == CameraMode::Orthographic)
    {
        const auto& view = GetViewport();
        return glm::ortho(view[0], view[1], view[2], view[3], _clip[0], _clip[1]);
    }

    return glm::mat4(1.0f);
}

TOON_ENGINE_API
void Camera::SetMode(CameraMode mode)
{
    _mode = mode;
}

TOON_ENGINE_API
void Camera::SetClip(const glm::vec2& clip)
{
    _clip = clip;
}

TOON_ENGINE_API
void Camera::SetUp(const glm::vec3& up)
{
    _up = up;
}

TOON_ENGINE_API
void Camera::SetForward(const glm::vec3& forward)
{
    if ((normalize(forward) + _up) == glm::vec3(0.0f))
    {
        SetOrientation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }
    else
    {
        SetOrientation(glm::quatLookAt(glm::normalize(forward), _up));
    }
}

TOON_ENGINE_API
glm::vec3 Camera::GetForward() const
{
    return glm::rotate(GetWorldOrientation(), GetWorldForward());
}

TOON_ENGINE_API
glm::vec3 Camera::GetRight() const
{
    return glm::normalize(glm::cross(GetForward(), GetUp()));
}

TOON_ENGINE_API
void Camera::SetAspect(float aspect)
{
    _aspect = aspect;
}

TOON_ENGINE_API
void Camera::SetAspect(const glm::vec2& size)
{
    _aspect = size.x / size.y;
}

TOON_ENGINE_API
void Camera::SetFOVX(float fovx)
{
    _fovX = fovx;
}

TOON_ENGINE_API
void Camera::SetFOVY(float fovy)
{
    _fovX = 2.0f * atanf(tanf(fovy * 0.5f) * _aspect);
}

TOON_ENGINE_API
void Camera::SetLookAt(const glm::vec3& point)
{
    SetForward(point - GetPosition());
}

TOON_ENGINE_API
void Camera::SetViewportSize(const glm::vec2& viewSize)
{
    _viewportSize = viewSize;
}

TOON_ENGINE_API
void Camera::SetViewportScale(const glm::vec4& viewScale)
{
    _viewportScale = viewScale;
}

TOON_ENGINE_API
glm::vec4 Camera::GetViewport() const
{
    glm::vec4 scale = GetViewportScale();
    glm::vec2 size = GetViewportSize();

    if (_aspect > 1.0f) {
        size.y /= _aspect;
    }
    else {
        size.x *= _aspect;
    }

    return glm::vec4(
        size.x * scale[0],
        size.x * scale[1],
        size.y * scale[2],
        size.y * scale[3]);
}


TOON_ENGINE_API
void Camera::SetDirection(glm::vec3 dir)
{
    _direction = dir;
}

TOON_ENGINE_API
void Camera::HandleMovement(float dt)
{
    float velocity = _movementSpeed * dt;

    glm::vec3 right = glm::normalize(glm::cross(_up, GetForward()));

    auto pos = GetPosition();

    _direction = glm::vec3(0.0f, 0.0f, 1.0f);//GetWorldForward();//glm::axisAngle(GetWorldForward(), GetWorldOrientation());

    pos += glm::normalize(glm::cross(GetForward(), _up)) * velocity * _direction.x;
    pos += glm::normalize(glm::cross(GetForward(), right)) * velocity * _direction.y;
    pos += GetForward() * velocity * _direction.z;

    SetPosition(pos);
}

TOON_ENGINE_API
void Camera::HandleRotation(float deltaX, float deltaY)
{
    glm::vec2 delta(deltaX, deltaY);

    delta *= _rotateSpeed;
    delta.x *= -1.0f;

    if (_invserse)
        delta.y *= -1.0f;

    glm::vec3 forward = GetForward();
    glm::vec3 right = glm::normalize(glm::cross(GetWorldUp(), forward));
    SetUp(glm::cross(forward, right));

    glm::quat rotation = GetOrientation();
    rotation = glm::angleAxis(delta.x, GetUp()) * rotation;
    rotation = glm::angleAxis(delta.y, GetRight()) * rotation;
    SetOrientation(rotation);
}

} // namespace Toon