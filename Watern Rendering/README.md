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

#### GLM
GLM is a header-only library, so you just need to include it in your project. Download it from [GLM's GitHub repository](https://github.com/g-truc/glm/releases) and include the glm directory in your project's include path.

#### GLEW

For Ubuntu-based systems, you can install GLEW using the following command:

```bash
sudo apt-get install libglew-dev
