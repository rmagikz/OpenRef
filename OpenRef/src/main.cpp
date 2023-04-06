#include <iostream>

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const int MaxQuads = 32;
const float ZoomSensitivity = 10.0f;

int windowWidth = 1280;
int windowHeight = 720;

glm::vec2 mouseOffset;
glm::dvec2 mousePos;

glm::mat4 proj(1.0f);

Quad quads[MaxQuads];
Quad* selectedQuad = nullptr;

bool isMovingQuad = 0;
bool isScalingQuad = 0;
bool isPanning = 0;

GLint quadCount = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            for (int i = 0; i < quadCount; i++)
            {
                Quad* current = &quads[i];
                if (Quad::IsOverlapBorder(mousePos, *current, 10.0f))
                {
                    selectedQuad = current;
                    isScalingQuad = 1;
                    return;
                }

                if (Quad::IsOverlap(mousePos, *current))
                {
                    std::cout << "MOVING QUAD" << std::endl;
                    selectedQuad = current;
                    mouseOffset = current->Position() - (glm::vec2)mousePos;

                    isMovingQuad = 1;
                    return;
                }
            }
            isPanning = 1;
            mouseOffset = (glm::vec2)mousePos - Quad::PositionOffset();
        }
    }

    if (action == GLFW_RELEASE)
    {
        isMovingQuad = 0;
        isScalingQuad = 0;
        isPanning = 0;
        selectedQuad = nullptr;
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float windowAspectRatio = (float)windowWidth / (float)windowHeight;

    float scaleDelta = yoffset * ZoomSensitivity;
    Quad::SetScaleOffset(Quad::ScaleOffset() + scaleDelta);
    Quad::ClampScaleOffset({ -100.0f, -100.0f }, { windowWidth, windowHeight });
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    glm::vec2 windowSizeDelta = { width - windowWidth, height - windowHeight };

    windowWidth = width;
    windowHeight = height;

    proj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);

    for (int i = 0; i < quadCount; i++)
    {
        Quad* current = &quads[i];

        current->SetPosition(current->Position() + windowSizeDelta / 2.0f);
    }
}

static void drop_callback(GLFWwindow* window, int path_count, const char* paths[])
{
    if (quadCount >= MaxQuads)
    {
        std::cout << "Max number of reference images reached!" << std::endl;
        return;
    }

    quads[quadCount] = { {windowWidth / 2, windowHeight / 2 }, (GLfloat)quadCount, paths[0]};

    quadCount++;
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
    glfwSetScrollCallback(window, scroll_callback);
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

        proj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
        shader.SetUniform4m("u_MVP", proj);

        GLint samplers[32]{};
        for (GLint i = 0; i < 32; i++)
        {
            samplers[i] = i;
        }
        shader.SetUniform1iv("u_Textures", 32, samplers);

        Renderer::Init();

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            mousePos = glm::clamp(mousePos, { 0.0, 0.0 }, { windowWidth, windowHeight });

            if (isMovingQuad)
            {
                selectedQuad->SetPosition((glm::vec2)mousePos + mouseOffset);
            }

            if (isPanning)
            {
                Quad::SetPositionOffset((glm::vec2)mousePos - mouseOffset);
            }

            if (isScalingQuad)
            {
                glm::vec2 scaleDelta = (glm::vec2)mousePos - selectedQuad->FinalPosition();
                glm::vec2 scaleOffset = (selectedQuad->Scale() - Quad::ScaleOffset()) * 0.5f;

                if (mousePos.x < selectedQuad->FinalPosition().x) scaleOffset *= -1.0f;
                   
                selectedQuad->SetScale(glm::abs(scaleDelta + scaleOffset));
                selectedQuad->ClampScale({ 150.0f, 150.0f }, { windowWidth, windowHeight });
                selectedQuad->CalculateAspectRatio();
            }


            Renderer::Begin();
            shader.SetUniform4m("u_MVP", proj);

            for (int i = 0; i < quadCount ; i++)
            {
                quads[i].GetTexture()->Bind(quads[i].TextureIndex());
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