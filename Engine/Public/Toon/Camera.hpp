#ifndef TOON_CAMERA_H
#define TOON_CAMERA_H

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Math.hpp>

#include <Toon/String.hpp>

namespace Toon {

enum CameraMode {
    Perspective,
    Orthographic,
}; // enum CameraMode

class TOON_ENGINE_API Camera : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Camera)
        
    Camera();

    virtual ~Camera();

    glm::mat4 GetView() const;

    glm::mat4 GetProjection() const;

    void SetMode(CameraMode mode);

    inline CameraMode GetMode() const {
        return _mode;
    }

    void SetClip(const glm::vec2& clip);

    inline glm::vec2 GetClip() const {
        return _clip;
    }

    void SetUp(const glm::vec3& up);

    inline glm::vec3 GetUp() const {
        return _up;
    }

    void SetForward(const glm::vec3& forward);

    glm::vec3 GetForward() const;

    glm::vec3 GetRight() const;

    void SetAspect (float aspect);
    
    void SetAspect(const glm::vec2& size);

    inline float GetAspect() const {
        return _aspect;
    }

    // Perspective

    void SetFOVX(float fovx);

    inline float GetFOVX() const {
        return _fovX;
    }

    void SetFOVY(float fovy);

    void SetLookAt(const glm::vec3& point);

    // Orthographic

    void SetViewportSize(const glm::vec2& viewSize);

    inline glm::vec2 GetViewportSize() const {
        return _viewportSize;
    }

    void SetViewportScale(const glm::vec4& viewScale);

    inline glm::vec4 GetViewportScale() const {
        return _viewportScale;
    }

    glm::vec4 GetViewport() const;

    // Movement
    void HandleMovement(float dt);
    void HandleRotation(float xoffset, float yoffset);

    void SetDirection(glm::vec3 dir);
    glm::vec3& GetDirection() { return _direction; }

private:

    CameraMode _mode = CameraMode::Perspective;

    glm::vec2 _clip = { 0.1f, 10000.0f };

    glm::vec3 _up = GetWorldUp();

    float _aspect = 16.0f / 9.0f; // 16:9

    unsigned _windowResizedEventHandlerID;

    // Perspective
    float _fovX = glm::radians(45.0f);

    // Orthographic
    glm::vec2 _viewportSize = glm::vec2(1920.0f, 1080.0f);

    glm::vec4 _viewportScale = glm::vec4(-0.5f, 0.5f, 0.5f, -0.5f);

    // Movement
    float _movementSpeed = 0.25f;

    float _rotateSpeed = 0.001f;

    bool _invserse = false;

    glm::vec3 _direction = glm::vec3(0);
};

} // namespace Toon

#endif // TOON_CAMERA_H
