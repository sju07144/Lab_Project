#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:
	void UpdateCameraVectors();
public:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 0.1f, 0.0f), 
		float _yaw = YAW, float _pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch = false);
	void ProcessMouseScroll (float yOffset);
};