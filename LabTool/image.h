#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <shader.h>
#include <stb_image.h>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

struct Point3
{
	Point3(float a, float b, float c) : x(a), y(b), z(c) { };
	Point3() { };
	float x, y, z;
};

class Image
{
public:
	Image(const std::string& fileName);
	Image(const Image& image) = delete;
	~Image();
	void Draw(Shader& shader);
	int GetWidth();
	int GetHeight();
private:
	unsigned char* data;
	cv::Mat img;
	int width;
	int height;
	int nrChannels;
	int faceNum;
	int pixelNum;
	int triplePixel;

	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint colorBufferID;
	GLuint normalBufferID;
	GLuint elementBufferID;
	
	/* Point3* normalMap;
	Point3* colors;
	Point3* vertices;
	GLuint* vertexElements; */
	std::vector<Point3> normalMap;
	std::vector<Point3> colors;
	std::vector<Point3> vertices;
	std::vector<GLuint> vertexElements;

	std::map<std::tuple<int, int, int>, int> posToIdx;

	void MakeNormalMap();
	void MakeVertices();
	void MakeVertexElements();
	void MakeColors();
	void SetupImages();
	float NormalizationMat2GL(int pix);
};