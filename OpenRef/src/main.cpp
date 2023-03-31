#include <iostream>

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Util.h"

const int MaxQuads = 1;

int windowWidth = 640;
int windowHeight = 480;

glm::vec2 windowSizeDelta;
glm::vec2 mouseOffset;
glm::dvec2 mousePos;

Texture** textures = new Texture*[MaxQuads];
Quad quads[MaxQuads];
Quad* selectedQuad = nullptr;

bool isMovingQuad = 0;
bool isScalingQuad = 0;
bool windowResized = 0;

GLint textureCount = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {

            for (int i = 0; i < MaxQuads; i++)
            {
                Quad* current = &quads[i];
                if (IsOverlapQuadCorner(mousePos, *current, 10.0f))
                {
                    selectedQuad = current;
                    isScalingQuad = 1;
                    return;
                }

                if (IsOverlapQuad(mousePos, *current))
                {
                    selectedQuad = current;
                    mouseOffset = current->Position - (glm::vec2)mousePos;

                    isMovingQuad = 1;
                    return;
                }
            }
        }

        if (action == GLFW_RELEASE)
        {
            isMovingQuad = 0;
            isScalingQuad = 0;
            selectedQuad = nullptr;
        }
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    windowSizeDelta = { width - windowWidth, height - windowHeight };

    windowWidth = width;
    windowHeight = height;

    windowResized = 1;
}

static void drop_callback(GLFWwindow* window, int path_count, const char* paths[])
{
    std::cout << paths[0] << std::endl;

    textures[textureCount] = new Texture(paths[0]);
    textures[textureCount]->Bind(textureCount);

    quads[textureCount] = { {windowWidth / 2, windowHeight / 2 }, { textures[textureCount]->Width(), textures[textureCount]->Height() }, (GLfloat)textureCount};

    textureCount++;
}

float randomFloat()
{
    return float(rand()) / float((RAND_MAX));
}

int main(void)
{

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    window = glfwCreateWindow(windowWidth, windowHeight, "OpenRef", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glClearColor(0.24f, 0.25f, 0.29f, 1.0f);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetDropCallback(window, drop_callback);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    {
        Shader shader("res/shaders/standard.vert", "res/shaders/standard.frag");
        shader.Bind();

        glm::mat4 proj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
        shader.SetUniform4m("u_MVP", proj);

        GLint textures[32];
        for (GLint i = 0; i < 32; i++)
        {
            textures[i] = i;
        }
        shader.SetUniform1iv("u_Textures", 32, textures);

        Renderer::Init();

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            mousePos = glm::clamp(mousePos, { 0.0, 0.0 }, { windowWidth, windowHeight });

            if (isMovingQuad)
            {
                selectedQuad->Position = (glm::vec2)mousePos + mouseOffset;
            }

            if (isScalingQuad)
            {
                glm::vec2 scaleDelta = (glm::vec2)mousePos - selectedQuad->Position;
                glm::vec2 scaleOffset = selectedQuad->Scale * 0.5f;

                if (mousePos.x < selectedQuad->Position.x) scaleOffset *= -1.0f;
                   
                selectedQuad->Scale = glm::abs(scaleDelta + scaleOffset);
                selectedQuad->Scale = glm::clamp(selectedQuad->Scale, {25.0f, 25.0f}, {windowWidth, windowHeight});
                selectedQuad->Scale.y = selectedQuad->Scale.x / selectedQuad->AspectRatio;
            }

            if (windowResized)
            {
                windowResized = 0;

                proj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
                shader.SetUniform4m("u_MVP", proj);

                for (int i = 0; i < MaxQuads; i++)
                {
                    Quad* current = &quads[i];

                    current->Position += windowSizeDelta / 2.0f;
                }
            }

            Renderer::Begin();

            for (int i = 0; i < textureCount; i++)
            {
                Renderer::DrawQuad(quads[i]);
            }
            
            Renderer::End();

            Renderer::Flush();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        Renderer::Shutdown();
    }

    for (int i = 0; i < textureCount; i++)
    {
        delete textures[i];
    }

    delete[] textures;

    glfwTerminate();
    return 0;
}