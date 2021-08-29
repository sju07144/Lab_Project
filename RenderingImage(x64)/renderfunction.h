#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>

/*
void Render*(); // Texture가 존재할 경우
void Render*WithColor(); // Color만 존재할 경우
*/

void RenderCube();
void RenderCubeWithColor();
void RenderQuad();
void RenderQuadWithColor();
void RenderRectangle(int width, int height);