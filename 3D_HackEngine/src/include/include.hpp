#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "figure.hpp"

#include <iostream>
#include "string"
#include <random>
#include <vector>

//const
const int WIDTH = 1280;
const int HEIGHT = 720;

const char* fragmentShaderSource = R"(
#version 460 core

uniform vec3 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
})";

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


//glfw
GLint shaderProgram;
GLint modellocation;
GLint viewlocation;
GLint projectionlocation;
GLint vertexColorlocation;
GLint currentx, currenty;
GLint xOffset, yOffset;
GLuint cVAO,cVBO,cEBO;
GLuint pVAO,pVBO,pEBO;
GLuint tVAO,tVBO,tEBO;

glm::mat4 count_figure[256];
glm::mat4 view,perspective;

glm::vec3 cameraPos(0.0f, 2.0f, 0.0f);
glm::vec3 cameraFront;
glm::vec3 worldUP(0.0f, 1.0f, 0.0f);


//float
float yaw = -90.0f;
float pitch = 0.0f;
float Speed;
float lastMouseX = float(WIDTH)/2;
float lastMouseY = float(HEIGHT)/2;
float Mouse_sensitivity = 0.1;

float aspect = (float)(WIDTH) / (float)(HEIGHT);


//vector 
std::vector<float> arr;


//bool
bool firstMouse = true;


struct rotate_val {
    float angle = 0;
    glm::vec3 axis;
};