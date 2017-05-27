#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

class Window;

class Camera
{
public:
    static Camera& instance()
    {
        static Camera* instance = new Camera();
        return *instance;
    }
    ~Camera(){};

    void Init(Window* aWindow, glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up);

    void Update(float dt);

    void Translate(glm::vec3 amount);

    glm::mat4 GetViewMat() { return _mViewMat; }
    glm::mat4 GetProjectionMat() { return _mProjectionMat; }
    glm::vec3 GetCameraPos() { return _mPosition; }
    void SetCameraPos(glm::vec3 position) { _mPosition = position; }
    glm::vec3                   GetCameraVelocity() { return _mVelocity; }
    void SetCameraVelocity(glm::vec3 velocity) { _mVelocity = velocity; }
	void AddForce(glm::vec3 force) { _mForce += force; }
	glm::vec2 GetResolution();

private:
    Camera(){};
    Window*   _mpWindow;
    glm::mat4 _mViewMat, _mProjectionMat;
    glm::vec3 _mPosition, _mVelocity, _mAcceleration, _mForce, _mForward, _mRight, _mUp, _mTarget, _mWorldUp;
    // clip coords created via - V clip = M projection * M view * M model * V local
};

#endif // CAMERA_H
