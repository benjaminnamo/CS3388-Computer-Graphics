# Textured Mesh Renderer
This program loads triangle mesh data and textures from files, renders them, and allows the user to explore the rendered world by moving the camera around via arrow keys. It is designed to help the user get comfortable with triangular meshes and working with the camera in world space.

## Installation
This program requires a C++ compiler and the OpenGL, GLFW, GLM, and GLEW libraries to be installed. To install OpenGL and GLFW on Ubuntu, you can use the following command:

```
sudo apt install libopengl-dev
sudo apt install libglfw3-dev
sudo apt install libglm-dev
sudo apt-get install libglew-dev
```
## Camera
The program uses a first-person camera controlled by the arrow keys. The camera begins positioned at (0.5, 0.4, 0.5) in world space and facing towards (0, 0, -1). The camera movements are as follows:

  - Up key: move the camera forward in the direction it is currently facing
  - Down key: move the camera backward in the exact opposite direction it is currently facing
  - Left key: rotate the camera counter-clockwise without moving
  - Right key: rotate the camera clockwise without moving
 The camera moves relatively smoothly. An algorithm is implemented to move the camera a certain amount for each frame a key is held down.

## File Reading
The program has a function that reads a file and returns a list of vertices and a list of faces. A VertexData class is implemented, which has instance variables for position (x, y, z), normal vector (nx, ny, nz), color (r, g, b), and texture coordinates (u, v). The class has to be implemented with at least a mandatory variable, i.e., position. A TriData class is also implemented with three instance variables representing the indices of the three vertices making up a triangle.

The readPLYFile function reads a PLY file and returns a list of VertexData objects and a list of TriData objects. The function is designed to handle any number of vertices with properties x, y, z, nx, ny, nz, red, green, blue, u, and v. It also handles faces with a property list vertex_indices. The function returns a tuple of lists in Python and uses the following function prototype to return a reference in C/C++:

```
void readPLYFile (std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces);
```
