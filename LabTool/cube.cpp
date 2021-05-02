#include "cube.h"

Cube::Cube(Shader& shader)
{
	// Initialize Vertices Data
	InitializeVertices();

	// Generate Vertex Array
	glGenVertexArrays(1, &cubeVertexArrayID);
	glBindVertexArray(cubeVertexArrayID);

	// Generate Vertex Buffer
	glGenBuffers(1, &cubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Transmit vertex data to shader program
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(offsetof(CubeVertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(offsetof(CubeVertex, texCoords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(offsetof(CubeVertex, tangent)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(offsetof(CubeVertex, bitangent)));

	diffuseMap = LoadTexture("brickwall.jpg");
	specularMap = LoadTexture("brickwall.jpg");
	shader.Use();
	shader.SetInt("material.diffuse", 0);
	shader.SetInt("material.specular", 1); 
	/* sphereMap = LoadTexture("sphere_map.jpg");
	shader.Use();
	shader.SetInt("sphereTexture", 0); */
}
Cube::~Cube()
{
	glDeleteBuffers(1, &cubeVertexBufferID);
	glDeleteVertexArrays(1, &cubeVertexArrayID);
}
void Cube::Draw(Shader& shader)
{
	shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap); 
	/* glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereMap); */

	glBindVertexArray(cubeVertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void Cube::InitializeVertices()
{
	// cube Vertices
	float cubeVertices[288] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	int cubeVerticesLength = sizeof(cubeVertices) / sizeof(int);

	for (int i = 0; i < cubeVerticesLength; i += 24)
	{
		CubeVertex vertex[3];
		for (int j = 0; j < 3; j++)
		{
			int temp = j << 3;
			glm::vec3 position;
			position.x = cubeVertices[i + temp];
			position.y = cubeVertices[i + temp + 1];
			position.z = cubeVertices[i + temp + 2];
			vertex[j].position = position;

			glm::vec3 normal;
			normal.x = cubeVertices[i + temp + 3];
			normal.y = cubeVertices[i + temp + 4];
			normal.z = cubeVertices[i + temp + 5];
			vertex[j].normal = normal;

			glm::vec2 texCoord;
			texCoord.x = cubeVertices[i + temp + 6];
			texCoord.y = cubeVertices[i + temp + 7];
			vertex[j].texCoords = texCoord;
		}

		glm::vec3 edge1 = vertex[1].position - vertex[0].position;
		glm::vec3 edge2 = vertex[2].position - vertex[0].position;
		glm::vec2 deltaUV1 = vertex[1].texCoords - vertex[0].texCoords;
		glm::vec2 deltaUV2 = vertex[2].texCoords - vertex[0].texCoords;

		float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		for (int j = 0; j < 3; j++)
		{
			vertex[j].tangent = tangent;
			vertices.push_back(vertex[j]);
		}
	}

}