#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, const Material& mats, GLuint faceNum)
{
	this->vertices = vertices;
	TransformPositions();
	this->indices = indices;
	this->textures = textures;
	this->mats = mats;
	this->faceNum = faceNum;
	SetupMesh();
	SetupColor();
}
void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) + sizeof(mats), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uniformBlockIndexID);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndexID);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndexID, 0, 3 * sizeof(glm::vec4) + sizeof(float));

	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndexID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(mats.Ka));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndexID);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mats.Kd));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndexID);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mats.Ks));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndexID);
	glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(float), &mats.transparent);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, bitangent)));
}
void Mesh::Draw(const Shader& shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint ambientNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;
	int count = 0;
	for (auto& i : textures)
	{
		glActiveTexture(GL_TEXTURE0 + count);

		std::string number;
		std::string name = i.type;
		if (name == "texture_diffuse")
			number = name + std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = name + std::to_string(specularNr++);
		else if (name == "texture_ambient")
			number = name + std::to_string(ambientNr++);
		else if (name == "texture_normal")
			number = name + std::to_string(normalNr++);
		else if (name == "texture_height")
			number = name + std::to_string(heightNr++);

		shader.SetFloat("material." + name + number, count);
		glBindTexture(GL_TEXTURE_2D, i.id);
		++count;
	}

	shader.SetUniformBlockBinding("Mat", 0);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vertexArrayID);
	// glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndexID, 0, sizeof(Material));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr); // if not nullptr, change to 0
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);
}
void Mesh::SetupColor()
{
	GLuint tripleFace = 3 * faceNum;
	colors.assign(tripleFace, glm::vec3(0.5f, 0.5f, 0.5f));
	glBindVertexArray(vertexArrayID);
	
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}
double Mesh::GetDist(glm::vec3 p1, glm::vec3 p2)
{
	float xDist = pow(p1.x - p2.x, 2);
	float yDist = pow(p1.y - p2.y, 2);
	float zDist = pow(p1.z - p2.z, 2);

	return sqrt(xDist + yDist + zDist);
}

double Mesh::BoundingBox(glm::vec3& boxCent)
{
	glm::vec3 maxCoordX(-9999.0f, -9999.0f, -9999.0f);
	glm::vec3 minCoordX(9999.0f, 9999.0f, 9999.0f);

	glm::vec3 maxCoordY(-9999.0f, -9999.0f, -9999.0f);
	glm::vec3 minCoordY(9999.0f, 9999.0f, 9999.0f);

	glm::vec3 maxCoordZ(-9999.0f, -9999.0f, -9999.0f);
	glm::vec3 minCoordZ(9999.0f, 9999.0f, 9999.0f);
	int num = 0;
	auto verticesSize = vertices.size();

	// 바운딩 박스의 8개 좌표들 구하기
	for (auto i = 0; i != verticesSize; i++)
	{
		glm::vec3 temp = vertices[i].position;

		if (maxCoordX.x < temp.x)
			maxCoordX = temp;

		if (minCoordX.x > temp.x)
			minCoordX = temp;

		if (maxCoordY.y < temp.y)
			maxCoordY = temp;

		if (minCoordY.y > temp.y)
			minCoordY = temp;

		if (maxCoordZ.z < temp.z)
			maxCoordZ = temp;

		if (minCoordZ.z > temp.z)
			minCoordZ = temp;
	}

	// 바운딩 박스의 중심의 좌표 구하기
	boxCent.x = (maxCoordX.x + minCoordX.x) / 2.f;
	boxCent.y = (maxCoordY.y + minCoordY.y) / 2.f;
	boxCent.z = (maxCoordZ.z + minCoordZ.z) / 2.f;

	// 바운딩 박스의 대각선 길이
	// maxCoordX를 기준으로 잡을 시, 절대 minCoordX와는 최대거리가 성립하지 않는다
	double maxDist = GetDist(maxCoordX, maxCoordY);
	double cmpDist = GetDist(maxCoordX, minCoordY);

	if (maxDist < cmpDist)
		maxDist = cmpDist;

	cmpDist = GetDist(maxCoordX, maxCoordZ);
	if (maxDist < cmpDist)
		maxDist = cmpDist;

	cmpDist = GetDist(maxCoordX, minCoordZ);
	if (maxDist < cmpDist)
		maxDist = cmpDist;

	return maxDist;
}
void Mesh::TransformPositions()
{
	glm::vec3 boundingCent;
	// Bounding Box
	double boundMaxDist = BoundingBox(boundingCent);

	float scalingSize = 1.8f / boundMaxDist;
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scalingSize, scalingSize, scalingSize));
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-boundingCent.x, -boundingCent.y, -boundingCent.z));
	glm::mat4 transformedMatrix = translateMatrix * scalingMatrix;

	// homogeneous coordinate
	auto verticesSize = vertices.size();
	for (auto i = 0; i != verticesSize; ++i)
	{
		glm::vec4 temp_homo(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, 1.0f); // (x,y.z.1 변환)
		float temp_output_homo[4] = { 0, };

		for (int j = 0; j < 3; ++j)
		{
			// 주의!
			// glm::mat4형을 디버그해보면 [0]~[3]은 열에 해당하는 정보를 담고 있었다. (x,y,z,w는 행에 해당하는 정보)
			if (j == 0)
			{
				temp_output_homo[j] += transformedMatrix[0].x * temp_homo.x;
				temp_output_homo[j] += transformedMatrix[1].x * temp_homo.y;
				temp_output_homo[j] += transformedMatrix[2].x * temp_homo.z;
				temp_output_homo[j] += transformedMatrix[3].x * temp_homo.w;
			}

			else if (j == 1)
			{
				temp_output_homo[j] += transformedMatrix[0].y * temp_homo.x;
				temp_output_homo[j] += transformedMatrix[1].y * temp_homo.y;
				temp_output_homo[j] += transformedMatrix[2].y * temp_homo.z;
				temp_output_homo[j] += transformedMatrix[3].y * temp_homo.w;
			}

			else  // j == 2
			{
				temp_output_homo[j] += transformedMatrix[0].z * temp_homo.x;
				temp_output_homo[j] += transformedMatrix[1].z * temp_homo.y;
				temp_output_homo[j] += transformedMatrix[2].z * temp_homo.z;
				temp_output_homo[j] += transformedMatrix[3].z * temp_homo.w;
			}
		}

		glm::vec3 output_homo(temp_output_homo[0], temp_output_homo[1], temp_output_homo[2]);
		vertices[i].position = output_homo;
	}
}