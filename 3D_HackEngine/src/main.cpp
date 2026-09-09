#include "include/API.hpp"

int main()
{

    GLFWwindow* window = API::Init_Engine(); //инициализация многих функций а также мохранение в буффер VAO,VBO,EBO, шейдеров и координат
    API::Init_Uniforms(); //инициализация переменных которые будут изменяться

    while (!glfwWindowShouldClose(window))
    {   
        API::Close_Window(window);

        API::Keys_Movement(window, 0);

        API::Clear_Screen(); 
        API::Vertex_Buffer();
        
        API::init_Camera();

        API::Create_Cube(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.0f, 1.0f);
        
        API::Create_Cube(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, 1.0f, 0.0f);

        API::Create_Platf(glm::vec3(-4.0f, -0.6f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), 0.4f, 0.4f, 0.4f);

        API::Create_Triangle(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 1.f, 1.0f, 1.0f);

        API::Limitation_View();

        API::UpdateFPS(window);

        API::Clear_Buffer(window);
    }

    API::Destroy_Engine(window);

    return 0;
}
