#pragma once

#include <GLFW/glfw3.h>
#include "figure.hpp"

#include "string"
#include <iostream>
#include <random>
#include <vector>

GLint shaderProgram;
GLint modellocation;
GLint viewlocation;
GLint projectionlocation;
GLint vertexColorlocation;

glm::mat4 count_figure[256];
std::vector<float> arr;

float yaw = -90.0f;
float pitch = 0.0f;

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

float randomFloat(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

namespace API{

void init_uniforms() {
    shaderProgram = createShaderProgram();

    modellocation = glGetUniformLocation(shaderProgram, "model");
    viewlocation = glGetUniformLocation(shaderProgram, "view");
    projectionlocation = glGetUniformLocation(shaderProgram,"projection");
    vertexColorlocation = glGetUniformLocation(shaderProgram,"vertexColor");
}

glm::vec3 getCameraFront(float angle){
    glm::vec3 front;

    front.x = cos(glm::radians(yaw+angle)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw+angle)) * cos(glm::radians(pitch));

    return glm::normalize(front);

}

glm::vec3 keys_Movement(GLFWwindow* window, char key, float value) {
    if (glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT)
    {
            return glm::vec3(0.05f, 0, 0.05f)*getCameraFront(value);
    }

    return glm::vec3();
}

int keys_See(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT)
    {
            if (key == GLFW_KEY_UP)
                return 1;
            if (key == GLFW_KEY_DOWN)
                return -1;
            if (key == GLFW_KEY_LEFT)
                return -1;
            if (key == GLFW_KEY_RIGHT)
                return 1;
    }

    return 0;
}

glm::vec3 keys_Hight(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT)
    {
            return glm::vec3(0.0f, 0.05f, 0.0f);
    }

    return glm::vec3();
}

void create_Cube(glm::mat4 view, glm::mat4 projection, glm::vec3 value, float r, float g, float b){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, value);

    glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(vertexColorlocation, r, g, b);

    glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(Figure::indices_cube) / sizeof(Figure::indices_cube[0])), GL_UNSIGNED_INT, nullptr);
}
};