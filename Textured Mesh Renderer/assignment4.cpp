// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

//function to control camera
void cameraControlsGlobe(glm::mat4& V) {
    static const float move_speed = 0.05f; // adjust this for desired movement speed
    static const float rotate_speed = 2.0f; // adjust this for desired rotation speed

    static glm::vec3 camera_position(0.5f, 0.4f, 0.5f);
    static glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    static glm::vec3 camera_up(0.0f, 1.0f, 0.0f);

    // handle keyboard input
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera_position += move_speed * camera_direction;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera_position -= move_speed * camera_direction;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera_direction = glm::rotate(camera_direction, glm::radians(rotate_speed), camera_up);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera_direction = glm::rotate(camera_direction, glm::radians(-rotate_speed), camera_up);
    }

    // update view matrix
    V = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
}

//provided load bitmap function
void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    // Actual RGBA data

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){
        printf("%s could not be opened. Are you in the right directory?\n", imagepath);
        getchar();
        return;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file1\n");
        fclose(file);
        return;
    }

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    *width      = *(int*)&(header[0x12]);
    *height     = *(int*)&(header[0x16]);
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file2\n");
        fclose(file);
        return;
    }
    // Make sure this is a 32bpp file
    if ( *(int*)&(header[0x1E])!=3  ) {
        printf("Not a correct BMP file3\n");
        fclose(file);
        return;
    }
    // fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
    // if ( *(int*)&(header[0x1C])!=32 ) {
    //     printf("Not a correct BMP file4\n");
    //     fclose(file);
    //     return;
    // }

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=(*width)* (*height)*4; // 4 : one byte for each Red, Green, Blue, Alpha component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    *data = new unsigned char [imageSize];

    if (dataPos != 54) {
        fread(header, 1, dataPos - 54, file);
    }

    // Read the actual data from the file into the buffer
    fread(*data,1,imageSize,file);

    // Everything is in memory now, the file can be closed.
    fclose (file);
}

//struct to hold vertex data
struct VertexData{
    GLfloat x,y,z,  //positions variables
    nx,ny,nz,       //normal vector
    r,g,b,          //color
    u,v;            //texture coords
};

//struct to hold indices of three vertices making up triangle
struct TriData{
    GLuint v1,v2,v3;
};

//function to read a .ply file
void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces) {
    std::ifstream input(fname);

    if (!input) {
        std::cerr << "Unable to open file " << fname << std::endl;
        exit(1);
    }

    std::string line;
    bool header = true;
    bool has_color = false;  //flag to indicate whether "color" property is declared
    int num_vertices = 0, num_faces = 0;

    while (getline(input, line)) {
        if (line == "end_header") {
            header = false;
            continue;
        }

        if (header) {
            std::string prefix, type;
            int count;
            std::stringstream ss(line);

            ss >> prefix >> type >> count;

            if (prefix == "element" && type == "vertex") {
                num_vertices = count;
            } else if (prefix == "element" && type == "face") {
                num_faces = count;
            } else if (prefix == "property" && type == "uchar" && line.find("red") != std::string::npos) {
                has_color = true;
            }
        } else {
            if (num_vertices > 0) {
                VertexData vertex;
                std::stringstream ss(line);
                ss >> vertex.x >> vertex.y >> vertex.z >> vertex.nx >> vertex.ny >> vertex.nz;
                if (has_color) {
                    ss >> vertex.r >> vertex.g >> vertex.b;
                }
                ss >> vertex.u >> vertex.v;
                vertices.push_back(vertex);
                num_vertices--;
            } else if (num_faces > 0) {
                TriData face;
                std::stringstream ss(line);
                int num_indices;
                ss >> num_indices;

                if (num_indices != 3) {
                    std::cerr << "Error: Only triangular faces are supported." << std::endl;
                    exit(1);
                }

                ss >> face.v1 >> face.v2 >> face.v3;

                faces.push_back(face);
                num_faces--;
            }
        }
    }
    input.close();
}

class TexturedMesh{

    std::vector<VertexData> vertices;   //vector storing vertices
    std::vector<TriData> faces;         //vector storing faces
    std::string ply_Path;
    std::string bmp_Path;

    GLuint vertex_posVBO,       //An integer ID for the VBO to store the vertex positions
    texture_coordVBO,           //An integer ID for the VBO to store the texture coordinates
    vertex_indexVBO,            //An integer ID for the VBO to store the face's vertex indices
    textureObj,                 //An integer ID for the Texture Object created to store the bitmap image
    meshVAO,                    //An integer ID for the VAO used to render the texture mesh
    programID;                  //An integer ID for the shader program created and linked to render the particular mesh.

    unsigned char* data;        //variable to store data from bmp
    unsigned int width, height; //variables to store size of bmps

public:

    TexturedMesh(std::string plyPath, std::string bmpPath){     //function takes in paths to ply and bmp as parameters

        ply_Path = plyPath;
        bmp_Path = bmpPath;

        loadARGB_BMP(bmp_Path.data(), &data, &width, &height);        //load bmp file into memory

        readPLYFile(ply_Path, vertices, faces);     //parse ply file parameter and store values in respective arrays


        //create mesh VAO
        glGenVertexArrays(1, &meshVAO);
        glBindVertexArray(meshVAO);

        //create vertex VBO
        int nVerts = vertices.size();
        glGenBuffers(1, &vertex_posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_posVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 11 * nVerts, &(vertices[0]), GL_STATIC_DRAW);  //use 11 since 11 values per struct
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           //normalized?
                sizeof(GL_FLOAT) * 11,  //stride -> 11 vals per struct
                (void*) 0               // buffer offset
        );

        //create texture coordinate VBO
        glGenBuffers(1, &texture_coordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texture_coordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 11 * nVerts, &(vertices[0]), GL_STATIC_DRAW);
        // 2nd attribute buffer : UV
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                sizeof(GL_FLOAT) * 11,            // stride
                (void*) 36 			              // data -> takes 9 values to get to uv, 9 x 4 bytes = 36
        );

        //create vertex index VBO
        glGenBuffers(1, &vertex_indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT) * 3 * faces.size(), &(faces[0]), GL_STATIC_DRAW);

        glBindVertexArray(0); //good practice to unbind VAO when done specifying vertices

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        std::string VertexShaderCode = "\
    	#version 330 core\n\
		// Input vertex data, different for all executions of this shader.\n\
		layout(location = 0) in vec3 vertexPosition;\n\
		layout(location = 1) in vec2 uv;\n\
		// Output data ; will be interpolated for each fragment.\n\
		out vec2 uv_out;\n\
		// Values that stay constant for the whole mesh.\n\
		uniform mat4 MVP;\n\
		void main(){ \n\
			// Output position of the vertex, in clip space : MVP * position\n\
			gl_Position =  MVP * vec4(vertexPosition,1);\n\
			// The color will be interpolated to produce the color of each fragment\n\
			uv_out = uv;\n\
		}\n";

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode = "\
		#version 330 core\n\
		in vec2 uv_out; \n\
		uniform sampler2D tex;\n\
		void main() {\n\
			gl_FragColor = texture(tex, uv_out);\n\
		}\n";

        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);

        // Compile Fragment Shader
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);

        programID = glCreateProgram();

        glAttachShader(programID, VertexShaderID);
        glAttachShader(programID, FragmentShaderID);
        glLinkProgram(programID);

        glDetachShader(programID, VertexShaderID);
        glDetachShader(programID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);


        //create texture obj
        glGenTextures(1, &textureObj);
        glBindTexture(GL_TEXTURE_2D, textureObj);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);      //changed this to RBGA instead of RGB
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void draw(glm::mat4 mvp){

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureObj);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(meshVAO);

        glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT,(void*) 0); //
        glBindVertexArray(0);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

};

int main( int argc, char* argv[]) {

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    // Open a window and create its OpenGL context
    float screenW = 1400;
    float screenH = 900;
    window = glfwCreateWindow( screenW, screenH, "Links House", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //create a textured mesh vector and store all of our .ply and .bmp information of objects in there
    std::vector<TexturedMesh> meshes;
    meshes.push_back(TexturedMesh("Walls.ply", "walls.bmp"));
    meshes.push_back(TexturedMesh("WoodObjects.ply", "woodobjects.bmp"));
    meshes.push_back(TexturedMesh("Table.ply", "table.bmp"));
    meshes.push_back(TexturedMesh("WindowBG.ply", "windowbg.bmp"));
    meshes.push_back(TexturedMesh("Patio.ply", "patio.bmp"));
    meshes.push_back(TexturedMesh("Floor.ply", "floor.bmp"));
    meshes.push_back(TexturedMesh("Bottles.ply", "bottles.bmp"));

    meshes.push_back(TexturedMesh("DoorBG.ply", "doorbg.bmp"));
    meshes.push_back(TexturedMesh("MetalObjects.ply", "metalobjects.bmp"));
    meshes.push_back(TexturedMesh("Curtains.ply", "curtains.bmp"));

    //depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.3f, 0.0f);

    glm::mat4 MVP;
    //projection matrix with a vertical field view of 45 degrees
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f);

    do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();

        // Projection = glm::mat4(1.0f);
        glLoadMatrixf(glm::value_ptr(Projection));

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glm::vec3 eye = {5.0f, 2.0f, 5.0f};
        // glm::vec3 eye = {-5.0f, 2.0f, -5.0f};
        glm::vec3 up = {0.0f, 1.0f, 0.0f};
        glm::vec3 center = {0.0f, 0.0f, 0.0f};
        //glm::mat4 V;

        glm::mat4 V = glm::lookAt(eye, center, up);

        glm::mat4 M = glm::mat4(1.0f);

        cameraControlsGlobe(V);                                        //camera movement

        glm::mat4 MV = V * M;
        glLoadMatrixf(glm::value_ptr(V));

        MVP = Projection * V * M;

        for (int i = 0; i < meshes.size(); i++){                           //call draw function for each texture mesh object
            meshes[i].draw(MVP);
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

