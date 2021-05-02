#include "camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	this->position = _position;
	this->worldUp = _up;
	this->yaw = _yaw;
	this->pitch = _pitch;
	UpdateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, 
	float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = _yaw;
	this->pitch = _pitch;
	UpdateCameraVectors();
}
glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(position, position + front, up); }
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	}
	UpdateCameraVectors();
}
void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constraintPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	UpdateCameraVectors();
}
void Camera::ProcessMouseScroll(float yOffset)
{
	zoom += static_cast<float>(yOffset);
	
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
	// UpdateCameraVectors();
}
void Camera::UpdateCameraVectors()
{
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}