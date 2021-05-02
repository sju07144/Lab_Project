#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Material
{
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
	float transparent;
};

struct Texture
{
	int id;
	std::string type;
	std::string path;
};

class Mesh
{
private:
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint elementBufferID;
	GLuint colorBufferID;
	GLuint uniformBlockIndexID;
	GLuint faceNum;
	void SetupMesh();
	void SetupColor();
	double GetDist(glm::vec3 p1, glm::vec3 p2);
	double BoundingBox(glm::vec3& boxCent);
	void TransformPositions();
public:
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Material mats;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& texCoords, const Material& mats, GLuint faceNum);
	void Draw(const Shader& shader);
	// void SetFaceNum(GLuint faceNum);
};