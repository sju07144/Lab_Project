#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>
#include <string>
#include <vector>

// normal map, albedo map 로딩
class Texture
{
public:
	Texture(const std::string& path, bool gammaCorrection = false);
	GLuint GetTextureID();
	int GetWidth();
	int GetHeight();
private:
	GLuint textureID;
	int width;
	int height;
	int nrComponents;
};

// output image 저장
bool SaveScreenshot(const std::string& fileName);
bool SaveScreenshotWithBMP(const std::string& fileName);