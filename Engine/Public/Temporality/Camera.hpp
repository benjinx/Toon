#ifndef TEMPORALITY_CAMERA_H
#define TEMPORALITY_CAMERA_H

#include <Temporality/Config.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/Math.hpp>

//class Window;

namespace Temporality {

enum CameraMode {
    Perspective,
    Orthographic,
};

class TEMPORALITY_ENGINE_API Camera : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Camera)
        
    Camera();

    virtual ~Camera() = default;

    // Methods

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const;

    void SetMode(CameraMode mode);
    inline CameraMode GetMode() const {
        return _mMode;
    }

    void SetAspect(float aspect);
    void SetAspect(const glm::vec2& size);

    inline float GetAspect() const {
        return _mAspect;
    }

    void SetFOVX(float fovx);
    void SetFOVY(float fovy);

    void SetViewportScale(const glm::vec4& viewScale);
    void SetViewportSize(const glm::vec2& viewSize);

    inline glm::vec4 GetViewportScale() const {
        return _mViewportScale;
    }

    inline glm::vec2 GetViewportSize() const {
        return _mViewportSize;
    }

    glm::vec4 GetViewport() const;

    void SetClip(const glm::vec2& clip);

    inline glm::vec2 GetClip() const {
        return _mClip;
    }

    void SetUp(const glm::vec3& up);

    inline glm::vec3 GetUp() const {
        return _mUp;
    }

    glm::vec3 GetRight() const;

    void SetForward(const glm::vec3& forward);

    glm::vec3 GetForward() const;

    void SetLookAt(const glm::vec3& point);

    void HandleMovement(float dt);
    void HandleRotation(float xoffset, float yoffset);

    //void SetCameraDirection(Direction direction);

    void SetDirection(glm::vec3 dir);
    glm::vec3& GetDirection() { return _mDirection; }

private:

    CameraMode _mMode = CameraMode::Perspective;

    glm::vec2 _mClip = glm::vec2(0.1f, 10000.0f);

    glm::vec3 _mUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float _mAspect = 16.0f / 9.0f; // 16:9

    // Perspective
    float _mFovX = glm::radians(45.0f);

    // Orthographic
    glm::vec2 _mViewportSize = glm::vec2(10.0f, 10.0f);
    glm::vec4 _mViewportScale = glm::vec4(-0.5f, 0.5f, 0.5f, -0.5f);

    float _mMovementSpeed = 0.25f;
    float _mRotateSpeed = 0.001f;

    bool _mInverse = false;

    glm::vec3 _mDirection = glm::vec3(0);
};

} // namespace Temporality

#endif // TEMPORALITY_CAMERA_H
