#include <iostream>

#include "Shader.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int windowWidth = 640;
int windowHeight = 480;

int widthDelta, heightDelta;

double xPos, yPos;
float xOffset, yOffset;

Quad quads[32];

Quad* selectedQuad = nullptr;

bool leftMousePressed = 0;
bool windowResized = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            for (int i = 0; i < 32; i++)
            {
                Quad* current = &quads[i];
                if (xPos > current->Position.x && xPos < current->Position.x + current->Scale.x &&
                    yPos > current->Position.y && yPos < current->Position.y + current->Scale.y)
                {
                    selectedQuad = current;
                    xOffset = current->Position.x - xPos;
                    yOffset = current->Position.y - yPos;
                }
            }

            if (selectedQuad == nullptr)
                return;

            leftMousePressed = 1;
        }

        if (action == GLFW_RELEASE)
        {
            leftMousePressed = 0;
            selectedQuad = nullptr;
        }
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    widthDelta = width - windowWidth;
    heightDelta = height - windowHeight;

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

        for (int i = 0; i < 32; i++)
        {
            quads[i] = { {randomFloat() * (windowWidth - 50.0f), randomFloat() * (windowHeight - 50.0f)}, {50.0f, 50.0f}, (GLfloat)i };
        }


        Renderer::Init();

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glfwGetCursorPos(window, &xPos, &yPos);
            xPos = glm::clamp(xPos, 0.0, (double)windowWidth);
            yPos = glm::clamp(yPos, 0.0, (double)windowHeight);

            if (leftMousePressed)
            {
                selectedQuad->Position.x = xPos + xOffset;
                selectedQuad->Position.y = yPos + yOffset;
            }

            if (windowResized)
            {
                windowResized = 0;

                proj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
                shader.SetUniform4m("u_MVP", proj);

                for (int i = 0; i < 32; i++)
                {
                    Quad* current = &quads[i];

                    current->Position.x += widthDelta / 2;
                    current->Position.y += heightDelta / 2;
                }
            }

            Renderer::Begin();

            for (int i = 0; i < 32; i++)
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