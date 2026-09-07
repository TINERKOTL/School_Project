#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/API.hpp"

#include <iostream>
#include <string>

const int WIDTH = 800;
const int HEIGHT = 600;

float aspect = (float)(WIDTH) / (float)(HEIGHT);

glm::vec3 cameraPos(0.0f, 2.0f, 0.0f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main()
{
    //
    if (!glfwInit()) {
        std::cerr << "Не удалось запустить GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);

    if (!window) {
        std::cerr << "Не удалось создать окно\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        std::cerr << "Не удалось загрузить OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Figure::vertices_cube), Figure::vertices_cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Figure::indices_cube), Figure::indices_cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Engine_Init() (API)
    API::init_uniforms();

    float time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {   
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glm::vec3 cameraFront = API::getCameraFront(0);

        glm::vec3 worldUP(0.0f, 1.0f, 0.0f);

        cameraPos += API::keys_Movement(window, 'W', 0);
        cameraPos += API::keys_Movement(window, 'A', -90);
        cameraPos += API::keys_Movement(window, 'S', 180);
        cameraPos += API::keys_Movement(window, 'D', 90);

        cameraPos += API::keys_Hight(window, GLFW_KEY_SPACE);
        cameraPos -= API::keys_Hight(window, GLFW_KEY_LEFT_CONTROL);

        pitch += API::keys_See(window, GLFW_KEY_UP);
        pitch += API::keys_See(window, GLFW_KEY_DOWN);
        yaw += API::keys_See(window, GLFW_KEY_LEFT);
        yaw += API::keys_See(window, GLFW_KEY_RIGHT);

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);            //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
                                                            // Clear_Screen() (API)
        glUseProgram(shaderProgram);                        //
        glBindVertexArray(VAO);                             //

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUP);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        API::create_Cube(view, projection, glm::vec3(-1.5f, 0.0f, 0.0f), 0.1f, 0.0f, 1.0f);
        API::create_Cube(view, projection, glm::vec3(-1.5f, 0.0f, 2.0f), 1.0f, 0.0f, 0.0f);
        API::create_Cube(view, projection, glm::vec3(1.5f, 0.0f, 2.0f), 0.0f, 1.0f, 1.0f);

        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    // Destroy_Engine() (API)
    return 0;
}
