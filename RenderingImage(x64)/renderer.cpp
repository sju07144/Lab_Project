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

    lightDir = glm::vec4(-1.0f, glm::sqrt(3.0f), -glm::sqrt(3.0f), 1.0f); // Directional Light�� ���⺤��
}
Renderer::~Renderer()
{
    glfwTerminate();
}
void Renderer::Render()
{
    InputModelName();
    InitializeTextureMap();
    InitializeShader();
    InitializeMatrix();
    RunLoop();
}
void Renderer::InitializeShader()
{
    shader.SetShader("Image.vshader", "Image.fshader"); // shader�� ���� �ۼ��� ���, ���ϸ� ����
    shader.Use();
    shader.SetInt("normalMap", 0); // ù ��° parameter�� �ִ� ���ڿ��� uniform ������ shader���� ���
    shader.SetInt("albedoMap", 1); // ���� ����
}
void Renderer::InputModelName()
{
    std::cout << "Input Model Name(resources: if arbitary): ";
    std::cin >> modelName;
    std::cout << "Input number of files: "; // �������� �ֽ� ������ ���� ��� �ַ� 12��, ȫ������ ��� �������� ���� ���� Ȯ�� �� �Է�
    std::cin >> fileSize;
}
void Renderer::InitializeTextureMap()
{
    if (modelName == "resources") // resources �������� �ҷ�����
    {
        for (int i = 0; i < fileSize; ++i)
        {
            normalMaps.push_back({ "resources/normal/" + std::to_string(i + 1) + ".gt(normal).png" });
            albedoMaps.push_back({ "resources/albedo/" + std::to_string(i + 1) + ".gt(albedo).png" });
            normalMapsForPrediction.push_back({ "resources/normal(prediction)/" + std::to_string(i + 1) + ".pred(normal).png" });
            albedoMapsForPrediction.push_back({ "resources/albedo(prediction)/" + std::to_string(i + 1) + ".pred(albedo).png" });
        }
    }
    else // modelName�� ���� sources ������ ���� �������� �ҷ�����
    {
        if (fileSize == 12) // �����Բ��� ���� ������ ������ ��� �� ���ڸ� �����ؾ���
        {
            for (int i = 0; i < fileSize; i++)
            {
                normalMaps.push_back({ "sources/" + modelName + "/normal/n_output_" + std::to_string(i) + ".png" });
                albedoMaps.push_back({ "sources/" + modelName + "/albedo/a_output_" + std::to_string(i) + ".png" });
            }
        }
        else
        {
            std::cerr << "Model not Found or file Size error\n";
            return;
        }
    }
}
void Renderer::InitializeMatrix()
{
    viewForLightDir = glm::lookAt(
        glm::vec3(3.0f, 4.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    projection = glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); // �� world coordinates�� �����ؾ���!!!
    view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, glm::sqrt(34.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    model = glm::mat4(1.0f);
}
void Renderer::RunLoop()
{
    bool isPrediction = false;
    int textureCount = 0;

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
        shader.SetVec4("lightDir", lightDir); 
        shader.SetMat4("viewForLightDir", viewForLightDir);

        if (modelName == "resources") // resources ������ ����
        {
            if (isPrediction)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, normalMapsForPrediction[textureCount].GetTextureID());
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, albedoMapsForPrediction[textureCount].GetTextureID());
                RenderQuad();
                SaveScreenshot("resources/image(prediction)/" + std::to_string(textureCount + 1) + ".pred(image).png");
            }
            else
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, normalMaps[textureCount].GetTextureID());
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, albedoMaps[textureCount].GetTextureID());
                RenderQuad();
                SaveScreenshot("resources/image/" + std::to_string(textureCount + 1) + ".gt(image).png");
            }

            if (isPrediction)
            {
                isPrediction = false;
                textureCount++;
            }
            else
                isPrediction = true;
        }

        else // sources ������ ����
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, normalMapsForPrediction[textureCount].GetTextureID());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, albedoMapsForPrediction[textureCount].GetTextureID());
            RenderQuad();
            SaveScreenshot("sources/" + modelName + "/image/i_output_" + std::to_string(textureCount) + ".png");
            textureCount++;
        }

        if (textureCount == fileSize)
            break;

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