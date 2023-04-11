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
## Camera Controls

The camera in this project provides a limited first-person view with the following controls:

* Click and drag the object:
  * Horizontal motion: Rotate the object around the vertical axis
  * Vertical motion: Rotate the object around the horizontal axis
* Arrow keys:
  * Up arrow key: Move the camera closer to the world origin
  * Down arrow key: Move the camera further away from the world origin

The initial camera position is set at (5, 5, 5) in world space and is looking at (0, 0, 0) in world space.

### Usage

1. Clone the repository or download the source code
2. Open the project in your development environment and ensure the include paths and linker settings are configured correctly for GLM, GLEW, and GLFW.
3. Compile and run the project.
