#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint programID;
	Shader(const char* vertexPath, const char* fragmentPath); // only vertex shader and fragment shader
	Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath); // add geometry shader
	Shader();

	void SetShader(const char* vertexPath, const char* fragmentPath);
	void SetShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec2(const std::string& name, float x, float y) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w = 1.0f) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

	void SetUniformBlockBinding(const std::string& name, GLuint uniformBlockBinding) const;
};
