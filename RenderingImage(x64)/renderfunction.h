#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>

/*
void Render*(); // Texture�� ������ ���
void Render*WithColor(); // Color�� ������ ���
*/

void RenderCube();
void RenderCubeWithColor();
void RenderQuad();
void RenderQuadWithColor();
void RenderRectangle(int width, int height);