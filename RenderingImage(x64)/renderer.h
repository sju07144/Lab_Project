#pragma once
#include <cmath>
#include <filesystem>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <renderfunction.h>
#include <shader.h>
#include <texture.h>
#include <textureWithClass.h>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Render();
private:
	GLFWwindow* window;
	const GLuint width = 256;
	const GLuint height = 256;

	const int fileSize = 63;

	Shader shader;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
	glm::mat4 viewForLightDir;

	glm::vec4 lightDir;
	glm::vec4 lightDirReal;

	std::vector<Texture> normalMaps;
	std::vector<Texture> albedoMaps;
	std::vector<Texture> normalMapsForPrediction;
	std::vector<Texture> albedoMapsForPrediction;

	std::string modelName;

	void InitializeShader();
	void InputModelName();
	void InitializeTextureMap();
	void InitializeMatrix();
	void RunLoop();

	void CreateDirectories();
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(GLFWwindow* Window);
};