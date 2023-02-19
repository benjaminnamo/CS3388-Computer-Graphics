#include <GLFW/glfw3.h>
#include <stdio.h>
#include "vector"
#include <math.h>

int screenWidth = 500;
int screenHeight = 500;
int numClicks = 0;

struct Point {  //structure for storing a point
    float x;
    float y;
};

struct Node : Point {   //structure for a Node
    bool hasHandle1, hasHandle2 ;
    bool actuallyHasHandle2;
    Point handle1 ;
    Point handle2 ;
    //bool isDragging = false;
};

float distance(Node A, Node B) {    //function for distance between two nodes in 2D
    float dx = B.x - A.x;
    float dy = B.y - A.y;
    return sqrt(dx * dx + dy * dy);
}

float distancePoint(float x1, float y1, float x2, float y2) {   //same function but for points
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

std :: vector <Node> nodes ;    //vector array to store our nodes
Node *draggingNode = NULL;      //temporary nodes to store nodes we are dragging on the screen
Node *draggingHandleA = NULL;
Node *draggingHandleB = NULL;

void mouse_callback(GLFWwindow* window, int button, int action, int mods) { //mouse_callback function to deal with mouse presses in general
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {     //while mouse is pressed
        double x, y;
        glfwGetCursorPos(window, &x, &y);                    //retrieve coordinates

        for (int i = 0; i < nodes.size(); i++) {                        //iterate through node array and compare coordinates with sensitivity of 5

            //checking if the location we clicked is a previous node
            if (distancePoint(nodes.at(i).x, nodes.at(i).y, x, screenHeight - y) <= 5) {
                draggingNode = &nodes[i];
            }
            //checking the same but for handle 1
            if (distancePoint(nodes.at(i).handle1.x, nodes.at(i).handle1.y, x, screenHeight - y) <= 5) {
                draggingHandleA = &nodes[i];
            }

            //checking the same but for handle 2
            if (nodes.at(i).hasHandle2) {
                if (distancePoint(nodes.at(i).handle2.x, nodes.at(i).handle2.y, x, screenHeight - y) <= 5) {
                    draggingHandleB = &nodes[i];
                }
            }
        }
        //if no nodes are currently being clicked/dragged we are adding a new node
        if (draggingNode == NULL && draggingHandleA == NULL && draggingHandleB == NULL) {
            Node tempNode;
            tempNode.x = x;
            tempNode.y = screenHeight - y;
            tempNode.hasHandle1 = true;
            tempNode.handle1.x = x;
            tempNode.handle1.y = screenHeight - y + 50;
            tempNode.hasHandle2 = false;
            tempNode.handle2.x = x;
            tempNode.handle2.y = screenHeight - y;

            if (nodes.size() > 1) {                                       //if we're adding a 3rd node or any node that creates an interior node
                float distance1 = distance(nodes[0], tempNode);     //comparing distance of new node to front and back of our node vector
                float distance2 = distance(nodes[nodes.size() - 1], tempNode);
                Point swapHandle;

                if ((distance1 - distance2)<0) {                          //the lesser value (closer proximity) determines where we place the new node
                    nodes.insert(nodes.begin(), tempNode);     //placing at the front requires insert
                    nodes[1].hasHandle2 = true;
                    nodes[1].actuallyHasHandle2 = true;
                    nodes[1].handle2.x = nodes[1].x + (nodes[1].x - nodes[1].handle1.x);
                    nodes[1].handle2.y = nodes[1].y + (nodes[1].y - nodes[1].handle1.y);
                    swapHandle = nodes[1].handle2;
                    nodes[1].handle2 = nodes[1].handle1;
                    nodes[1].handle1 = swapHandle;

                } else {
                    nodes.push_back(tempNode);                          //adding to the back done through push_back
                    nodes[nodes.size() - 2].hasHandle2 = true;
                    nodes[nodes.size() - 2].actuallyHasHandle2 = true;
                    nodes[nodes.size() - 2].handle2.y = nodes[nodes.size() - 2].y - 50;
                    nodes[nodes.size() - 2].handle2.x = nodes[nodes.size() - 2].x + (nodes[nodes.size() - 2].x - nodes[nodes.size() - 2].handle1.x);
                    nodes[nodes.size() - 2].handle2.y = nodes[nodes.size() - 2].y + (nodes[nodes.size() - 2].y - nodes[nodes.size() - 2].handle1.y);
                    swapHandle = nodes[nodes.size() - 2].handle2;
                    nodes[nodes.size() - 2].handle2 = nodes[nodes.size() - 2].handle1;
                    nodes[nodes.size() - 2].handle1 = swapHandle;

                }
            } else {
                nodes.push_back(tempNode);                              //if it's our first or second node, pushback.
            }
            numClicks++;                                                //increment number of elements (from before I figured out how vectors work but I'll leave it since its harmless.)
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {    //if we're done dragging/clicking our objects, we can set them to NULL again.
        draggingNode = NULL;
        draggingHandleA = NULL;
        draggingHandleB = NULL;
    }
}

void dragNode(GLFWwindow* window){  //function for dragging a Node
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    float deltaX = x - draggingNode->x; //store difference between new x and y coords of node vs. old ones
    float deltaY = (screenHeight - y) - draggingNode->y;

    draggingNode->x = x;                //set nodes new coords
    draggingNode->y = screenHeight - y;

    draggingNode->handle1.x = draggingNode->handle1.x + deltaX; //set handle 1's new coords by applying the difference
    draggingNode->handle1.y = draggingNode->handle1.y + deltaY;

    if (draggingNode->actuallyHasHandle2) { //set handle 2's new coords by applying the difference. or no. we calculate it relatively.
        draggingNode->handle2.x = draggingNode->x + (draggingNode->x - draggingNode->handle1.x);
        draggingNode->handle2.y = draggingNode->y + (draggingNode->y - draggingNode->handle1.y);
    }
}

void dragHandleA(GLFWwindow* window){   //function for dragging handle 1/handle A
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    draggingHandleA->handle1.x = x;
    draggingHandleA->handle1.y = screenHeight - y;
    draggingHandleA->handle2.x = draggingHandleA->x + (draggingHandleA->x - draggingHandleA->handle1.x);
    draggingHandleA->handle2.y = draggingHandleA->y + (draggingHandleA->y - draggingHandleA->handle1.y);
}

void dragHandleB(GLFWwindow* window){   //function for dragging handle 2/handle B
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    draggingHandleB->handle2.x = x;
    draggingHandleB->handle2.y = screenHeight - y;
    draggingHandleB->handle1.x = draggingHandleB->x + (draggingHandleB->x - draggingHandleB->handle2.x);
    draggingHandleB->handle1.y = draggingHandleB->y + (draggingHandleB->y - draggingHandleB->handle2.y);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {    //callback for keyboard clicks
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
            nodes.clear();
    }
}

void drawPoint(Point drawPoint){                    //function to draw a new node
    glVertex2f(drawPoint.x,drawPoint.y);
}

float bezier4(float p0, float p1, float p2, float p3, float t) { //ugly function to calculate the bezier curve between two nodes, two critical points, given t sections.
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    float p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt * p3;
    return p;
}

void renderCurves(){    //function to render our curves in the window
    for (int j = 0; j < nodes.size() - 1; j++) {
        for (int i = 0; i < 200; ++i) {
            float t = (float)i;
            t /= 200;
            // Connect the previous vertex to the current vertex
            if (i > 1) {
                glVertex2f(bezier4(nodes[j].x, nodes[j].handle1.x, nodes[j + 1].handle2.x, nodes[j + 1].x, t - 1.0f / 200),
                           bezier4(nodes[j].y, nodes[j].handle1.y, nodes[j + 1].handle2.y, nodes[j + 1].y, t - 1.0f / 200));
            }
            glVertex2f(bezier4(nodes[j].x, nodes[j].handle1.x, nodes[j + 1].handle2.x, nodes[j + 1].x, t),
                       bezier4(nodes[j].y, nodes[j].handle1.y, nodes[j + 1].handle2.y, nodes[j + 1].y, t));
        }
    }
}

void renderStipple(){   //function to render the stipple between points and handles
    for (int i = 0; i <nodes.size(); i++) {
        glVertex2f(nodes.at(i).handle1.x,nodes.at(i).handle1.y);
        glVertex2f(nodes.at(i).x, nodes.at(i).y);

        if (nodes.at(i).hasHandle2){
            glVertex2f(nodes.at(i).handle2.x,nodes.at(i).handle2.y);
            glVertex2f(nodes.at(i).x, nodes.at(i).y);
        }
    }
}

void renderHandles(){   //function to render handles
    for (int i = 0; i <nodes.size(); i++) {
        drawPoint(nodes.at(i).handle1);

        if (nodes.at(i).hasHandle2){
            drawPoint(nodes.at(i).handle2);
        }
    }
}

void renderPoints(){    //function to render points
    for (int i = 0; i <nodes.size(); i++) {
        drawPoint(nodes.at(i));
    }
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);  //Create a window with 4 times multisampling enabled

    if (argc >= 2){ //if no arguments, default screen size is set to 500x500. otherwise, width and height arguments are passed
        screenWidth = atoi(argv[1]);
        screenHeight = atoi(argv[2]);
    }

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Spline Tool", NULL, NULL);

    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_MULTISAMPLE);   //enable multisampling

    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); //setting viewport
    glViewport(0, 0, screenWidth, screenHeight);

    //Main loop
    while (!glfwWindowShouldClose(window)) {

        if (draggingHandleA != NULL) {      //if were dragging handle 1, call respective function
            dragHandleA(window);
        }

        if (draggingHandleB != NULL) {      //similarly
            dragHandleB(window);
        }

        if (draggingNode != NULL) {         //similarly
          dragNode(window);
        }

        //Clear the screen
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0,0,0);
        glfwSetMouseButtonCallback(window, mouse_callback);
        glfwSetKeyCallback(window, key_callback);

        //render curves
        glEnable (GL_LINE_SMOOTH );
        glEnable (GL_BLEND );
        glBlendFunc (GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
        if (nodes.size() > 1) {
            glLineWidth(1.5f);
            glBegin(GL_LINES);
            renderCurves();
            glEnd();
        }
        glDisable(GL_LINE_SMOOTH );
        glDisable(GL_BLEND );

        //lines from points to handles
        glEnable ( GL_LINE_STIPPLE );
        glLineStipple (1 , 0x00FF );
        glBegin ( GL_LINES );
        glColor3f(1,0,0);
        renderStipple();
        glEnd ();
        glDisable( GL_LINE_STIPPLE );
        glColor3f(0,0,0);

        //Render points we click
        glPointSize (15);
        glDisable(GL_MULTISAMPLE);
        glBegin( GL_POINTS );
        renderPoints();
        glEnd ();
        glEnable(GL_MULTISAMPLE);

        //render handles
        glEnable(GL_POINT_SMOOTH );
        glEnable(GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
        glBegin(GL_POINTS);
        renderHandles();
        glEnd();
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_BLEND);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //Terminate GLFW
    glfwTerminate();
    return 0;
}