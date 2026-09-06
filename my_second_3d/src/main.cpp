#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

const int WIDTH = 800;
const int HEIGHT = 600;

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPos(0.0f, 0.0f, 1.0f);

const char* vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    vertexColor = aColor;
})";

const char* fragmentShaderSource = R"(
#version 460 core

uniform vec3 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
})";


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log((std::size_t)logLength, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "Shader compilation failed:\n" << log << '\n';
        glDeleteShader(shader);
        return 0;
    }

    return shader;
};

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (vertexShader == 0 || fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string log((std::size_t)logLength, '\0');
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::cerr << "Shader program linking failed:\n" << log << '\n';
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

glm::vec3 getCameraFront(float angle){
    glm::vec3 front;

    front.x = cos(glm::radians(yaw+angle)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw+angle)) * cos(glm::radians(pitch));

    return glm::normalize(front);

}

int main()
{
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

    if (!gladLoadGL(
            reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        std::cerr << "Не удалось загрузить OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    float vertices[] =
    {
//      x      y      z     r     g     b
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    };

    unsigned int indices[] =
    {
        //передняя грань
        0, 1, 2, 2, 3, 0,

        //задняя грань
        4, 6, 5, 4, 7, 6,

        //левая грань
        0, 3, 7, 0, 7, 4,

        //правая грань
        1, 5, 6, 1, 6, 2,

        //верхняя грань
        2, 6, 7, 2, 7, 3,

        //нижняя грань
        0, 4, 5, 0, 5, 1

    };

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLint shaderProgram = createShaderProgram();
    if (shaderProgram == 0)
    {
        std::cerr << "Проблема с запуском шейдеров";
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLint modellocation = glGetUniformLocation(shaderProgram, "model");
    GLint viewlocation = glGetUniformLocation(shaderProgram, "view");
    GLint projectionlocation = glGetUniformLocation(shaderProgram,"projection");
    GLint vertexColorlocation = glGetUniformLocation(shaderProgram,"vertexColor");

    while (!glfwWindowShouldClose(window))
    {   
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glm::vec3 cameraFront = getCameraFront(0);

        glm::vec3 worldUP(0.0f, 1.0f, 0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
        {
            cameraPos += glm::vec3(0.05f, 0, 0.05f)*getCameraFront(0);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT)
        {
            cameraPos += glm::vec3(0.05f, 0, 0.05f)*getCameraFront(-90);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
        {
            cameraPos += glm::vec3(0.05f, 0, 0.05f)*getCameraFront(180);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT)
        {
            cameraPos += glm::vec3(0.05f, 0, 0.05f)*getCameraFront(90);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_REPEAT)
        {
            cameraPos += glm::vec3(0, 0.05f, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT)
        {
            cameraPos -= glm::vec3(0, 0.05f, 0);
        }


        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_REPEAT)
        {
            pitch += 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_REPEAT)
        {
            pitch -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_REPEAT)
        {
            yaw -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_REPEAT)
        {
            yaw += 1.0f;
        }

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        float time = glfwGetTime();
        float aspect = (float)(WIDTH) / (float)(HEIGHT);

        //glm::mat4 view = glm::mat4(1.0f);
        //view = glm::translate(view, glm::vec3(view_pos_x,view_pos_y,view_pos_z));
        //view = glm::rotate(view, 1.0f, glm::vec3(view_angle_x,view_angle_y,view_angle_z));

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUP);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        //первый куб
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(-1.5f, 0.0f, 0.0f));
        //model1 = glm::rotate(model1, time*2, glm::vec3(0.0f,0.6f,0.6f));

        glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model1));
        glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(vertexColorlocation, 1.0f, 0.0f, 0.0f);

        glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(indices) / sizeof(indices[0])), GL_UNSIGNED_INT, nullptr);


        //второй куб
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(-1.5f, 0.0f, 2.0f));
        //model2 = glm::rotate(model2, time*2, glm::vec3(0.5f,1.0f,0.3f));

        glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(vertexColorlocation, 0.0f, 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(indices) / sizeof(indices[0])), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
