#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <shader.h>
#include <texture.h>
#include <vector>

struct CubeVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Cube
{
public:
    Cube(Shader& shader);
    ~Cube();
    void Draw(Shader& shader);
private:
    void InitializeVertices();
    std::vector<CubeVertex> vertices;
    GLuint cubeTangentBufferID;
	GLuint cubeVertexArrayID;
	GLuint cubeVertexBufferID;
    GLuint diffuseMap;
    GLuint specularMap;
    GLuint normalMap;
    GLuint sphereMap;
};