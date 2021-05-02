#pragma once
#include <fstream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>
#include <string>
#include <vector>

GLuint LoadTexture(const char* path, bool gammaCorrection = false);
GLuint LoadTextureUsingDirectory(const char* path, const std::string& directory, bool gamma = false);
GLuint LoadCubemap(const std::vector<std::string>& faces);
bool SaveScreenshot(const std::string& fileName);
bool SaveScreenshotWithBMP(const std::string& fileName);