#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec2 aPosition;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 460 core

out vec4 FragColor;

//uniform vec3 trianglecolor

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";



void framebufferSizeCallback(
    GLFWwindow* window,
    int width,
    int height
)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Не удалось запустить GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "OpenGL Triangle",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Не удалось создать окно\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGL(
            reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        std::cerr << "Не удалось загрузить OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL: "
              << glGetString(GL_VERSION)
              << '\n';

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] =
    {
//      x   y
        0,  0,
        1,  0,
        1,  1,

        0,  0,
        0,  1,
        1,  1, 
    };

    std::cout << vertices << std::endl;

    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        nullptr
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//    GLint colorlocation = glGetUniformLocation(
//        shaderProgram,
//        "triangleсolor"
//    );


    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // glUniform3f(colorlocation, 1.0f, 0.0f, 0.0f);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // glUniform3f(colorlocation, 1.0f, 0.3f, 1.0f);
        glDrawArrays(GL_TRIANGLES,0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
