#ifndef CAMERA_H
#define CAMERA_H

#include <Config.hpp>
#include <GameObject.hpp>
#include <Math.hpp>

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

class Camera : public GameObject
{
public:

	enum Mode {
		Perspective,
		Orthographic,
	};

	//DISALLOW_COPY_AND_ASSIGN(Camera);
	
	Camera();

	virtual ~Camera() = default;

	// Methods

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

	void SetMode(Mode mode);
	Mode GetMode() const { return _mMode; }

	void SetAspect(float aspect);
	void SetAspect(const glm::vec2& size);

	inline float GetAspect() const { return _mAspect; }

	void SetFOVX(float fovx);
	void SetFOVY(float fovy);

	void SetViewportScale(float left, float right, float bottom, float top);
	void SetViewportScale(const glm::vec4& viewScale);

	glm::vec4 GetViewportScale() const { return _mViewportScale; }

	void SetViewportSize(float width, float height);
	void SetViewportSize(const glm::vec2& viewSize);

	glm::vec2 GetViewportSize() const { return _mViewportSize; }

	glm::vec4 GetViewport() const;

	void SetClip(float near, float far);
	void SetClip(const glm::vec2& clip);
	inline glm::vec2 GetClip() const { return _mClip; }

	void SetUp(const glm::vec3& up);
	glm::vec3 GetUp() { return _mUp; }
	glm::vec3 GetRight() { return glm::normalize(glm::cross(GetForward(), GetUp())); }

	void SetForward(const glm::vec3& forward);
	glm::vec3 GetForward() const;

	void SetLookAt(const glm::vec3& point);

	void SetAutoResize(bool autoResize);

	void HandleMovement(Direction dir, float dt);
	void HandleRotation(float xoffset, float yoffset);

private:

	Mode _mMode = Mode::Perspective;

	bool _mAutoResize = true;

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
};

#endif // CAMERA_H
