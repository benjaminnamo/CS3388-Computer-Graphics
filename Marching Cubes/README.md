# Marching Cubes - 3D Surface Reconstruction
This code is an implementation of the Marching Cubes algorithm, which is used for generating a 3D triangular mesh from a scalar field (a 3D function). The algorithm marches through the 3D space, creating triangles by examining the field values at the corners of cubes.

## Why this approach
The Marching Cubes algorithm is a popular method for generating 3D meshes due to its simplicity and effectiveness. This implementation includes GLFW and GLEW for OpenGL support, and GLM for matrix operations.

## Known Bugs
There are no known bugs at this time.

## Installation
This program requires a C++ compiler and the OpenGL, GLFW, GLM, and GLEW libraries to be installed. To install OpenGL, GLFW, GLM, and Glew on Ubuntu, you can use the following command:

```
sudo apt install libopengl-dev
sudo apt install libglfw3-dev
sudo apt install libglm-dev
sudo apt-get install libglew-dev
```
Also, make sure you have downloaded shaders.hpp, TriTable.hpp, and CubeAxes.hpp.

## Main Features
Marching Cubes mesh generation
Camera controls for orbiting around the mesh
Vertex normals computation for shading
Optional PLY file output for saving the generated mesh
Different sample 3D functions for testing

## Usage
Use arrow keys to move the camera around the mesh
Click and drag the left mouse button to rotate the camera
Press 's' to save the mesh as a PLY file
To change the 3D function, comment/uncomment the desired function in the f function in the code. To adjust the step size, min, max coordinates, iso-value, or output file name, modify the corresponding variables in the main function.

Please note that the performance may vary depending on the size of the mesh and the number of triangles generated.
