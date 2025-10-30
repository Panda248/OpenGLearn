// Code following the tutorial from https://learnopengl.com/, starting from creating a window to creating a shader program

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

const int FPS = 60;

/*
* Vertices of a triangle we want to render. Notice z for all vertices are 0. This is a 2D triangle
*/
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

/*
* Vertices of another triangle we want to render
*/

float otherVertices[] = {
    -0.5f, -0.5f, 0.0f,
    -0.4f, -0.5f, 0.0f,
     -0.5f,  0.5f, 0.0f,
};

/*
* Vertices of two triangles stored within one array
*/
float twoTriVertices[] = {
    -0.4f, -0.2f, 0.0f,
     0.4f, -0.2f, 0.0f,
     0.0f,  0.4f, 0.0f,

     0.4f, 0.2f, 0.0f,
     -0.4f, 0.2f, 0.0f,
     0.0f,  -0.4f, 0.0f
};

/*
* Lets make a rectangle
* To elminate overhead, store unique vertices and then create triangles using the index of each vertices.
* This geometry will be rendered using an element buffer object
*/
float rectVertices[] = {
    -0.2f, -0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f,  0.2f, 0.0f,
     0.2f,  0.2f, 0.0f
};

unsigned int rectIndices[] = {
    0,1,2,
    1,2,3
};



//Vertex Shader Code 
const char* vertexShaderSource =
"#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader Code
const char* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"} \0";

//Other Fragment Shade Code
const char* otherFragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 0.5f, 0.2f, 0.01f);\n"
"} \0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int not_main()
{
    /*
    * Instantiating GLFW Window
    * Initialize GLFW
    * Configure OpenGL version as 3.3 (organized as MAJOR.MINOR)
    */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MacOS X specific call

    /*
    * Creating window object
    */
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learning OpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    /*
    * Register functions to GLFW callbacks (resize window/viewport, process input changes, process error messages, etc.)
    * These must be after window is created and before the render loop
    */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);


    /*
    * GLAD: load OpenGL function pointers
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // <CREATING SHADER PROGRAM>

    //Creates a Vertex Shader Object
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Create Fragment Shader Object
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Create Fragment Shader Object based on different code
    unsigned int oFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(oFragmentShader, 1, &otherFragmentShaderSource, NULL);
    glCompileShader(oFragmentShader);

    //Test Compiled correctly
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //glGetShaderiv gets specified info on shader

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(oFragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(oFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::OTHERFRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Create Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Test success
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);

    //Create another Shader Program
    unsigned int otherShaderProgram = glCreateProgram();
    glAttachShader(otherShaderProgram, vertexShader);
    glAttachShader(otherShaderProgram, oFragmentShader);
    glLinkProgram(otherShaderProgram);

    glGetProgramiv(otherShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(otherShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::OTHERSHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Delete shader objects to free up space
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(oFragmentShader);

    // </CREATING SHADER PROGRAM>

    // <CREATING SHADER PROGRAM FROM FILE>
    Shader shaderObj("shaders/vertex.glsl", "shaders/fragment.glsl");
    // </CREATING SHADER PROGRAM FROM FILE>

    // <CREATING A TRIANGLE>

    //Create Vertex Array Object for triangle
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //Creates Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind Buffer Object to its type (GL_ARRAY_BUFFER)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //glBufferData copies data to buffer memory which then copies to GPU memory
    //Specify which part of input data goes to what vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // </CREATING A TRIANGLE>


    // <CREATING A RECTANGLE>

    //Create Vertex Array Object for Rectangle
    unsigned int RVAO;
    glGenVertexArrays(1, &RVAO);
    glBindVertexArray(RVAO);


    //Create Rectangle Vertex Buffer Object
    unsigned int RVBO;
    glGenBuffers(1, &RVBO);
    glBindBuffer(GL_ARRAY_BUFFER, RVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    //Create Element Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//note the ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

    //Specify which part of input data goes to what vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // </CREATING A RECTANGLE>

    //// <CREATING TWO TRIANGLES FROM ONE VERTEX ARRAY>
    //unsigned int TVAO;
    //glGenVertexArrays(1, &TVAO);
    //glBindVertexArray(TVAO);

    //unsigned int TVBO;
    //glGenBuffers(1, &TVBO);
    //glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriVertices), twoTriVertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// </CREATING TWO TRIANGLES FROM ONE VERTEX ARRAY>

    // <CREATING TWO TRIANGLES FROM TWO VERTEX ARRAYS>
    //uncomment the original triangle code
    unsigned int OVAO;
    glGenVertexArrays(1, &OVAO);
    glBindVertexArray(OVAO);

    unsigned int OVBO;
    glGenBuffers(1, &OVBO);
    glBindBuffer(GL_ARRAY_BUFFER, OVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(otherVertices), otherVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Wireframe rendering
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*
    * Render Loop
    * Swap color buffer then polls for events like keyboard or mouse input
    */

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Straight Opacity
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //Premult opacity

    //Sets clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    //If you don't want to clear in the render loop for some reason, clear both buffers or else u get epilepsy
    //glClear(GL_COLOR_BUFFER_BIT);
    //glfwSwapBuffers(window);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    float time;
    float r;
    float g;
    float b;
    int colorID = glGetUniformLocation(shaderProgram, "color");



    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        r = (sin(time) * 2.0f) + 0.5f;
        g = (3.0f*sin(time) * 2.0f) + 0.5f;
        b = (cos(time) * 2.0f) + 0.5f;
        glUniform4f(shaderProgram, r, g, b, 1.0f);

        processInput(window);

        //glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(RVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(otherShaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glBindVertexArray(OVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //glBindVertexArray(TVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



/*
* Resizes viewport when window is resized
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
* Input processing
*/
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/*
* Listening to key events. This is good for stuff like on release
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //Listening for GLFW_RELEASE is like onkeyreleased in game engines
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
