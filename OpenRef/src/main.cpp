#include <iostream>

#include "Shader.h"
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

Quad quads[MaxQuads];
Quad* selectedQuad = nullptr;

bool isMovingQuad = 0;
bool isScalingQuad = 0;
bool windowResized = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {

            for (int i = 0; i < MaxQuads; i++)
            {
                Quad* current = &quads[i];
                if (IsOverlapQuadCorner(mousePos, *current, 5.0f))
                {
                    selectedQuad = current;
                    mouseOffset = current->Position - (glm::vec2)mousePos;
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

        for (int i = 0; i < MaxQuads; i++)
        {
            quads[i] = { {randomFloat() * (windowWidth - 50.0f), randomFloat() * (windowHeight - 50.0f)}, {50.0f, 50.0f}, (GLfloat)i };
        }


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
                selectedQuad->Scale = (glm::vec2)mousePos - selectedQuad->Position;
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

            for (int i = 0; i < MaxQuads; i++)
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

    glfwTerminate();
    return 0;
}