#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <texture.h>
#include <shader.h>
#include <model.h>
#include <camera.h>
#include <iostream>
#include <vector>

class Renderer
{
public:
	Renderer(GLuint width, GLuint height);
	~Renderer();
	void RunLoop();
private:
	GLFWwindow* window;
	GLuint mWidth;
	GLuint mHeight;
	Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) }; // Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)) -> compile error
	float lastX;
	float lastY;
	bool firstMouse;

	float deltaTime;
	float lastFrame;

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	void ProcessInput(GLFWwindow* Window);
};