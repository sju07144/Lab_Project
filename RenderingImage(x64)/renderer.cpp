#include "renderer.h"

Renderer::Renderer()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        getchar();
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Image-based Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorial.\n";
        getchar();
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        getchar();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    lightDir = glm::vec4(1.0f, 1.0f, 3.0f, 1.0f);
}
Renderer::~Renderer()
{
    glfwTerminate();
}
void Renderer::Render()
{
    InitializeShader();
    InputModelName();
    InitializeTextureMap();
    InitializeMatrix();
    RunLoop();
}
void Renderer::InitializeShader()
{
    shader.SetShader("Image.vshader", "Image.fshader");
    shader.Use();
    shader.SetInt("normalMap", 0);
    shader.SetInt("albedoMap", 1);
}
void Renderer::InputModelName()
{
    std::cout << "Input Model Name: ";
    std::cin >> modelName;
}
void Renderer::InitializeTextureMap()
{
    for (int i = 0; i < 12; i++)
    {
        normalMaps.push_back({ "sources/" + modelName + "/normal/n_output_" + std::to_string(i) + ".png" });
        albedoMaps.push_back({ "sources/" + modelName + "/albedo/a_output_" + std::to_string(i) + ".png" });
    }
}
void Renderer::InitializeMatrix()
{
    viewForLightDir = glm::lookAt(
        glm::vec3(3.0f, 4.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // lightDir tranform
    /* for (int i = 0; i < 4; ++i)
    {
        int sum = 0;
        for (int j = 0; j < 4; ++j)
        {
            sum += viewForLightDir[j][i] * lightDir[j];
        }
        lightDirReal[i] = sum;
    } */


    projection = glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); // 꼭 world coordinates로 설정해야함!!!

    /* view = glm::lookAt(
        glm::vec3(3.0f, 4.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    ); */
    view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, glm::sqrt(34.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    model = glm::mat4(1.0f);
    /* model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-43.314f), glm::vec3(1.0f, 0.0f, 0.0f)); */
}
void Renderer::RunLoop()
{
    GLuint textureCount = 0;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);
        shader.SetMat4("model", model);

        shader.SetVec3("viewPos", glm::vec3(0.0f, 0.0f, glm::sqrt(34.0f)));
        // shader.SetVec3("lightDir", glm::vec3(1.0f, 1.0f, 3.0f));
        shader.SetVec4("lightDir", lightDir);
        shader.SetMat4("viewForLightDir", viewForLightDir);
        // shader.SetVec3("lightDirNormalized", glm::normalize(glm::vec3(1.0f, 1.0f, 3.0f)));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, normalMaps[textureCount].GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, albedoMaps[textureCount].GetTextureID());
        RenderQuad();
        SaveScreenshot("sources/" + modelName + "/image/i_output_" + std::to_string(textureCount) + ".png");
        // SaveScreenshot("D://temp" + std::to_string(textureCount) + ".png");
        // SaveScreenshot("D://temp.png");
        // SaveScreenshot("directional_light/plate/i_output_" + std::to_string(textureCount) + ".png");
        // SaveScreenshot("prediction_destination/output(gt_modification)_" + std::to_string(lightPosCount) + ".png");

        textureCount++;
        if (textureCount % 12 == 0)
        {
            glfwTerminate();
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
void Renderer::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}