#pragma once

#include "include.hpp"
#include <utility>


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

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec3 getCameraFront(float angle){
        glm::vec3 front;

        front.x = cos(glm::radians(yaw+angle)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw+angle)) * cos(glm::radians(pitch));

        return glm::normalize(front);

}

void init_cube() {
        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &cVAO);
        glGenBuffers(1, &cVBO);
        glGenBuffers(1, &cEBO);

        glBindVertexArray(cVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Figure::vertices_cube), Figure::vertices_cube, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Figure::indices_cube), Figure::indices_cube, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
}

void init_paral() {
        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &pVAO);
        glGenBuffers(1, &pVBO);
        glGenBuffers(1, &pEBO);

        glBindVertexArray(pVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Figure::vertices_paral), Figure::vertices_paral, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Figure::indices_paral), Figure::indices_paral, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
}

void init_triangle() {
        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &tVAO);
        glGenBuffers(1, &tVBO);
        glGenBuffers(1, &tEBO);

        glBindVertexArray(tVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Figure::vertices_triangle), Figure::vertices_triangle, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Figure::indices_triangle), Figure::indices_triangle, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
}

void Camera_key(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_REPEAT)
            pitch += 0.8*Speed;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_REPEAT)
            pitch -= 0.8*Speed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_REPEAT)
            yaw -= 0.8*Speed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_REPEAT)
            yaw += 0.8*Speed;
}

void Camera_mouse(GLFWwindow* window, double xpos, double ypos){
    currentx = xpos;
    currenty = ypos;

    if (firstMouse) {
        lastMouseX = currentx;
        lastMouseY = currenty;
        firstMouse = false;
    }

    xOffset = currentx - lastMouseX;
    yOffset = lastMouseY - currenty;

    lastMouseX = currentx;
    lastMouseY = currenty;

    yaw += xOffset*Mouse_sensitivity;
    pitch += yOffset*Mouse_sensitivity;

    getCameraFront(0);
}


namespace API{

    void Init_Uniforms() {
        shaderProgram = createShaderProgram();

        modellocation = glGetUniformLocation(shaderProgram, "model");
        viewlocation = glGetUniformLocation(shaderProgram, "view");
        projectionlocation = glGetUniformLocation(shaderProgram,"projection");
        vertexColorlocation = glGetUniformLocation(shaderProgram,"vertexColor");
    }

    int Keys_Movement(GLFWwindow* window, int value) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT)
            Speed = 2.0f;
        else 
            Speed = 1.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
                cameraPos += glm::vec3(0.03f, 0, 0.03f)*glm::normalize(glm::vec3(0.03f, 0, 0.03f)*getCameraFront(0))*glm::vec3(Speed*2);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT)
                cameraPos += glm::vec3(0.03f, 0, 0.03f)*glm::normalize(glm::vec3(0.03f, 0, 0.03f)*getCameraFront(-90))*glm::vec3(Speed*2);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
                cameraPos += glm::vec3(0.03f, 0, 0.03f)*glm::normalize(glm::vec3(0.03f, 0, 0.03f)*getCameraFront(180))*glm::vec3(Speed*2);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT)
                cameraPos += glm::vec3(0.03f, 0, 0.03f)*glm::normalize(glm::vec3(0.03f, 0, 0.03f)*getCameraFront(90))*glm::vec3(Speed*2);
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_REPEAT)
                cameraPos += glm::vec3(0.0f, 0.05f, 0.0f);
        
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT)
                cameraPos -= glm::vec3(0.0f, 0.05f, 0.0f);

        if (value == 0) {
            glfwSetCursorPosCallback(window, Camera_mouse);
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else 
            Camera_key(window);

        return 0;
    }

    void Close_Window(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void Create_Cube(glm::vec3 translate_value, glm::vec3 scale, float r, float g, float b){

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translate_value);
        model = glm::scale(model, scale);

        glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform3f(vertexColorlocation, r, g, b);

        glBindVertexArray(cVAO);

        glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(Figure::indices_cube) / sizeof(Figure::indices_cube[0])), GL_UNSIGNED_INT, nullptr);
    }

    void Create_Platf(glm::vec3 translate_value, glm::vec3 scale, float r, float g, float b){

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translate_value);
        model = glm::scale(model, scale);

        glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform3f(vertexColorlocation, r, g, b);

        glBindVertexArray(pVAO);

        glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(Figure::indices_paral) / sizeof(Figure::indices_paral[0])), GL_UNSIGNED_INT, nullptr);
    }

    void Create_Triangle(glm::vec3 translate_value, glm::vec3 scale, float r, float g, float b){

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translate_value);
        model = glm::scale(model, scale);

        glUniformMatrix4fv(modellocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewlocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform3f(vertexColorlocation, r, g, b);

        glBindVertexArray(tVAO);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
    }

    GLFWwindow* Init_Engine() {

        if (!glfwInit()) {
            std::cerr << "Не удалось запустить GLFW\n";
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Triangle", nullptr, nullptr);

        if (!window) {
            std::cerr << "Не удалось создать окно\n";
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
        {
            std::cerr << "Не удалось загрузить OpenGL\n";
            glfwDestroyWindow(window);
            glfwTerminate();
        }


        init_cube();
        init_paral();
        init_triangle();

        return window;
    }

    void Clear_Screen() {
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);            
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    }

    void Destroy_Engine(GLFWwindow* window) {
        glDeleteVertexArrays(1, &cVAO);
        glDeleteVertexArrays(1, &pVAO);
        glDeleteVertexArrays(1, &tVAO);
        glDeleteBuffers(1, &cVBO);
        glDeleteBuffers(1, &pVBO);
        glDeleteBuffers(1, &tVBO);
        glDeleteBuffers(1, &cEBO);
        glDeleteBuffers(1, &pEBO);
        glDeleteBuffers(1, &tEBO);
        glDeleteProgram(shaderProgram);

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Vertex_Buffer() {
        glUseProgram(shaderProgram);                          
    }

    void Clear_Buffer(GLFWwindow* window) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    float GetTime() {
        return glfwGetTime();
    }

    glm::mat4 Create_Camera() {
        cameraFront = getCameraFront(0);
        return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUP);
    }

    glm::mat4 Create_Perspective() {
        return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    }

    void Limitation_View() {
        pitch = glm::clamp(pitch, -88.0f, 89.0f);
    }

    void UpdateFPS(GLFWwindow* window) {
        static double previousTime = glfwGetTime();
        static int frameCount = 0;

        double currentTime = glfwGetTime();
        frameCount += 1;

        double elaspedTime = currentTime - previousTime;

        if (elaspedTime >= 1.0) {
            double fps = frameCount / elaspedTime;

            std::string title = "OpenGL | FPS: " + std::to_string((int)fps);

            glfwSetWindowTitle(window, title.c_str());

            frameCount = 0;
            previousTime = currentTime;
        }
    }

    void init_Camera() {
        view = API::Create_Camera();
        perspective = API::Create_Perspective();
    }
};