#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <camera.h>
#include <renderfunction.h>
#include <shader.h>
#include <texture.h>
#include "textureWithClass.h"

GLFWwindow* window;
const GLuint width = 256;
const GLuint height = 256;

Camera camera(glm::vec3(0.0f, 0.0f, 2.4f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float degree = 0.0f;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* Window);

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
// glm::vec3 lightPos;
std::vector<glm::vec3> lightPos;

/* GLuint normalMap;
GLuint albedoMap; */

std::vector<Texture> normalMaps;
std::vector<Texture> albedoMaps;

int main(void)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        // fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorial.\n";
        // fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorial.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        // fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    /* glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

    /* lightPos.push_back(glm::vec3(-3.0f, 3.5f, 4.0f));
    lightPos.push_back(glm::vec3(-3.0f, 4.0f, 3.5f));
    lightPos.push_back(glm::vec3(3.5f, -3.0f, 4.0f));
    lightPos.push_back(glm::vec3(3.5f, 4.0f, -3.0f));
    lightPos.push_back(glm::vec3(4.0f, 3.5f, -3.0f));
    lightPos.push_back(glm::vec3(4.0f, -3.0f, 3.5f)); */

    lightPos.push_back(glm::vec3(-6.0f, 2.5f, 5.0f));
    lightPos.push_back(glm::vec3(-6.0f, 5.0f, 2.5f));
    lightPos.push_back(glm::vec3(2.5f, -6.0f, 5.0f));
    lightPos.push_back(glm::vec3(2.5f, 5.0f, -6.0f));
    lightPos.push_back(glm::vec3(5.0f, 2.5f, 5.0f));
    lightPos.push_back(glm::vec3(5.0f, -6.0f, 2.5f));

    Shader shader("image.vshader", "image.fshader");
    // lightPos = glm::vec3(100.0f, 50.0f, 80.0f);
    shader.Use();
    shader.SetInt("normalMap", 0);
    shader.SetInt("albedoMap", 1);

    /* for (int i = 0; i < 17; i++)
    {
        normalMaps.push_back({ "sources/airplane(sources)/n_output_" + std::to_string(i) + ".png" });
        albedoMaps.push_back({ "sources/airplane(sources)/a_output_" + std::to_string(i) + ".png" });
    } */
    normalMaps.push_back({ "prediction_source/gt(normal).png" });
    albedoMaps.push_back({ "prediction_source/gt(albedo).png" });

    GLuint lightPosCount = 0;
    GLuint textureCount = 0;

    // projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    projection = glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); // 꼭 world coordinates로 설정해야함!!!
    view = glm::lookAt(
        glm::vec3(3.0f, 4.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    // view = camera.GetViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-43.314f), glm::vec3(1.0f, 0.0f, 0.0f));


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);
        shader.SetMat4("model", model);
        // shader.SetVec3("viewPos", camera.position);
        shader.SetVec3("viewPos", glm::vec3(3.0f, 4.0f, 3.0f));
        shader.SetVec3("lightPos", lightPos[lightPosCount]);
        // shader.SetVec3("lightDir", glm::vec3(1.0f, 1.0f, 3.0f));
        // shader.SetVec3("lightDir", glm::vec3(1, 1, 3));
        shader.SetVec3("lightDirNormalized", glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, normalMaps[textureCount].GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, albedoMaps[textureCount].GetTextureID());
        RenderQuad();
        // SaveScreenshot("sources/plate(results)/i_output_" + std::to_string(textureCount + lightPosCount * 17) + ".png");
        // SaveScreenshot("D://temp" + std::to_string(textureCount) + ".png");
        // SaveScreenshot("D://temp.png");
        // SaveScreenshot("directional_light/plate/i_output_" + std::to_string(textureCount) + ".png");
        SaveScreenshot("prediction_destination/output(gt_modification)_" + std::to_string(lightPosCount) + ".png");
        lightPosCount++;
        if (lightPosCount == 6)
            lightPosCount = 0;

       // std::cout << textureCount + lightPosCount * 17 << '\n';

        /* textureCount++;
        if (textureCount % 17 == 0)
        {
            textureCount = 0;
            lightPosCount++;
            if (lightPosCount == 6)
            {
                lightPosCount = 0;
            }
        } */
        /* textureCount++;
        if (textureCount % 17 == 0)
        {
            textureCount = 0;
        } */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(yOffset);
}