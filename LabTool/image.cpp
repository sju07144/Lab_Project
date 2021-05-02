#define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
#include "image.h"

Image::Image(const std::string& fileName)
{
	img = cv::imread(fileName.c_str(), cv::IMREAD_UNCHANGED);
	cv::cvtColor(img, img, cv::COLOR_BGRA2RGB);

	data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	// faceNum = width * height * 26;
	faceNum = 2 * (width - 1) * (height - 1);
	// pixelNum = width * height * 13;
	pixelNum = width * height;
	triplePixel = 3 * pixelNum;

	/* normalMap = new Point3[faceNum];
	vertices = new Point3[pixelNum];
	colors = new Point3[pixelNum];
	vertexElements = new GLuint[3 * faceNum]; */

	normalMap.reserve(pixelNum);
	vertices.reserve(pixelNum);
	colors.reserve(pixelNum);
	vertexElements.reserve(3 * faceNum);

	MakeNormalMap();
	MakeVertices();
	MakeColors();
	MakeVertexElements();

	SetupImages();
}
Image::~Image()
{
	/* delete[] normalMap;
	delete[] vertices;
	delete[] colors;
	delete[] vertexElements; */

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &colorBufferID);
	glDeleteBuffers(1, &normalBufferID);
	glDeleteBuffers(1, &elementBufferID);

	glDeleteVertexArrays(1, &vertexArrayID);
}
void Image::Draw(Shader& shader)
{
	shader.Use();
	glBindVertexArray(vertexArrayID);
	// glDrawArrays(GL_TRIANGLES, 0, width * height * 3);
	glDrawElements(GL_TRIANGLES, vertexElements.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
void Image::SetupImages()
{
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	/* glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, pixelNum * sizeof(Point3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, pixelNum * sizeof(Point3), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, faceNum * sizeof(Point3), &normalMap[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * faceNum * sizeof(GLuint), &vertexElements[0], GL_STATIC_DRAW); */

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Point3), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normalMap.size() * sizeof(Point3), &normalMap[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElements.size() * sizeof(GLuint), &vertexElements[0], GL_STATIC_DRAW);
}
void Image::MakeVertices()
{
	int idx = 0;
	Point3 vertex;
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			// 하나의 사각형 면(두 개의 삼각형)을 구성하는 좌표들 넣넣
			// 출발 기준점은 (k,j,0)
			// 가로로 이동해가며 한행씩 넣는걸로 index가 지정됨
			vertex.x = static_cast<float>(j);
			vertex.y = static_cast<float>(i);
			vertex.z = 0.0f;
			posToIdx[{j, i, 0}] = ++idx;
			vertices.push_back(vertex);
		}
	}
}
void Image::MakeColors()
{
	Point3 color;
	for (int i = 0; i < pixelNum; ++i)
	{
		color.x = 0.0f;
		color.y = 1.0f;
		color.z = 0.0f;
		colors.push_back(color);
	}
}
void Image::MakeNormalMap()
{
	int normalVertexIndex = 0;
	Point3 normal;
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			float res[3] = { 0.0f, };
			for (int k = 0; k < 3; k++)
			{
				res[k] = NormalizationMat2GL(img.at<cv::Vec3b>(i, j)[k]);
			}
			normal.x = res[0];
			normal.y = res[1];
			normal.z = res[2];
			normalMap.push_back(normal);
			// ++normalVertexIndex;
		}
	}
}
void Image::MakeVertexElements()
{
	int vertex_idx = 0;
	int triangle_num = 0;
	int triangle_idx = 0;

	int dx[] = { 0, 1, 0, -1 };
	int dy[] = { 1, 0, -1, 0 };


	for (int j = 0; j < height; j++) {
		for (int k = 0; k < width; k++) {
			int nx[4], ny[4];   //R, B, L, T
			for (int l = 0; l < 4; ++l) {
				nx[l] = l > 1 ? k + 1 + dy[l] : k + dy[l];
				ny[l] = l > 1 ? j + 1 + dx[l] : j + dx[l];
			}
			if (0 <= nx[0] && nx[0] < width && 0 <= ny[0] && ny[0] < height
				&& 0 <= nx[1] && nx[1] < width && 0 <= ny[1] && ny[1] < height) {
				vertexElements.push_back(posToIdx[{k, j, 0}]);
				vertexElements.push_back(posToIdx[{nx[0], ny[0], 0}]);
				vertexElements.push_back(posToIdx[{nx[1], ny[1], 0}]);
				/* vertexElements[triangle_idx++] = posToIdx[{k, j, 0}];
				vertexElements[triangle_idx++] = posToIdx[{nx[0], ny[0], 0}];
				vertexElements[triangle_idx++] = posToIdx[{nx[1], ny[1], 0}]; */
				++triangle_num;
			}
			if (0 <= nx[2] && nx[2] < width && 0 <= ny[2] && ny[2] < height
				&& 0 <= nx[3] && nx[3] < width && 0 <= ny[3] && ny[3] < height
				&& 0 <= j + 1 && j + 1 < height && 0 <= k + 1 && k + 1 < width) {
				vertexElements.push_back(posToIdx[{nx[3], ny[3], 0}]);
				vertexElements.push_back(posToIdx[{k + 1, j + 1, 0}]);
				vertexElements.push_back(posToIdx[{nx[2], ny[2], 0}]);
				/* vertexElements[triangle_idx++] = posToIdx[{nx[3], ny[3], 0}];
				vertexElements[triangle_idx++] = posToIdx[{k + 1, j + 1, 0}];
				vertexElements[triangle_idx++] = posToIdx[{nx[2], ny[2], 0}]; */
				++triangle_num;
			}
		}
	}
}
float Image::NormalizationMat2GL(int pix) 
{
	float res = (static_cast<float>(pix) - 127.0f) / 127.0f;
	return res < 0 ? std::max(-1.0f, res) : std::min(1.0f, res);
}
int Image::GetWidth() { return width;  }
int Image::GetHeight() { return height;  }