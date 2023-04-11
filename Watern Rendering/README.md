# Basic Water Shader Project

This project demonstrates the implementation of a basic water shader using tessellation, geometry shaders, and displacement maps. The result is a realistic patch of water rendered on-screen.

![Water Shader Screenshot](screenshot.png)

## Features

* Input: Simple mesh of triangles
* Tessellated mesh for enhanced details
* Geometry shader for wave manipulation using Gerstner waves
* Realistic textures and depth added to waves
* Camera options: Third-person or globe movement

## Getting Started

These instructions assume you have basic knowledge of OpenGL and GLSL.

### Prerequisites

To compile and run the project, you need to have:

* OpenGL compatible GPU
* A development environment with support for OpenGL and GLSL
* Installed libraries: GLM, GLEW, GLFW

### Installing Dependencies
This program requires a C++ compiler and the OpenGL, GLFW, GLM, and GLEW libraries to be installed. To install OpenGL, GLFW, GLM, and Glew on Ubuntu, you can use the following command:

```
sudo apt install libopengl-dev
sudo apt install libglfw3-dev
sudo apt install libglm-dev
sudo apt-get install libglew-dev
```
