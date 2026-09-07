# School_Project
Make school project on C++, use OpenGL + GLFW + GLAD how graphic library

Main file - main.cpp
API flie - API.hpp

Packages and Library:
glfw - 3.5.1-1
glad - 2.0.8
mesa - 26.2.2-1
glm - 1.0.3-1

sudo pacman -S glfw glad mesa glm //on Arch
sudo apt update && sudo apt install glfw glad mesa glm //on Debian/Ubuntu

To launch the project:
cmake -S . -B build 
cmake --build build
./build/Triangle
