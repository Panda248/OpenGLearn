// Code following the tutorial from https://learnopengl.com/, starting from creating a window to creating a shader program

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Shader.h"

int view_width = 800;
int view_height = 600;
float aspect = (float)view_width / (float)view_height;

float speed = 1;
float uOpacity = 0.1f;
float uFloor = 0.1f;

unsigned int FBO;
unsigned int texture;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    /*
    * Instantiating GLFW Window
    * Initialize GLFW
    * Configure OpenGL version as 4.5 (organized as MAJOR.MINOR)
    */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MacOS X specific call

    // Creating window object
    GLFWwindow* window = glfwCreateWindow(view_width, view_height, "Line Trails", NULL, NULL);

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

    // GLAD: load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader lineShader("shaders/lineTrailVert.glsl", "shaders/lineTrailFrag.glsl");
    int timeUniformLocation = glGetUniformLocation(lineShader.ID, "u_time");
    int aspectUniformLocation = glGetUniformLocation(lineShader.ID, "u_aspect_ratio");

    Shader clearShader("shaders/vertex2d.glsl", "shaders/fade.frag");
    int opacityUniformLocation = glGetUniformLocation(clearShader.ID, "u_opacity");

    Shader quadShader("shaders/texVert.glsl", "shaders/texFragFloor.glsl");
    int textureUniformLocation = glGetUniformLocation(quadShader.ID, "u_Texture");
    int floorUniformLocation = glGetUniformLocation(quadShader.ID, "u_floor");

    // setup array for storing line vertices
    // array format: x, y, r, offset
    int lineCount = 10;
    float minOffset = 0;
    float maxOffset = glm::pi<float>();
    float minR = 0.1, maxR = 1;
    std::vector<float> lines(lineCount * 4 * 2);
    for (int i = 0; i < lineCount; i++) {
        float x = (((float)rand() / (float)RAND_MAX)) - 0.5f;
        float y = (((float)rand() / (float)RAND_MAX)) - 0.5f;
        float radius = (((float) rand() / (float)RAND_MAX) * (float)(maxR - minR)) + minR;
        float offset = (((float)rand() / (float)RAND_MAX) * (float)(maxOffset - minOffset)) + minOffset;

        int index = i * 8;

        lines[index] = x;
        lines[index + 1] = y;
        lines[index + 2] = radius;
        lines[index + 3] = offset;

        lines[index + 4] = x;
        lines[index + 5] = y;
        lines[index + 6] = radius;
        lines[index + 7] = offset + 0.1 * speed;
    }

    for (int i = 0; i < lines.size(); i+=4) {
        printf("%1.3f, %1.3f, %1.3f, %1.3f\n", lines[i], lines[i + 1], lines[i + 2], lines[i + 3]);
    }



    unsigned int lineVBO;
    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lineCount * 8 * sizeof(float), &lines[0], GL_STATIC_DRAW);

    unsigned int lineVAO;
    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);
    
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // radius attribute
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // offset attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Clear quad
    float clear[] = {
        -1,-1,
        -1, 1,
         1, -1,
         1, 1
    };

    // Texture Quad
    float quad[] = {
        -1, -1, 0, 0,
        -1,  1, 0, 1,
         1, -1, 1, 0,
         1,  1, 1, 1
    };

    unsigned int clearVBO;
    glGenBuffers(1, &clearVBO);
    glBindBuffer(GL_ARRAY_BUFFER, clearVBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), clear, GL_STATIC_DRAW);

    unsigned int clearVAO;
    glGenVertexArrays(1, &clearVAO);
    glBindVertexArray(clearVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), quad, GL_STATIC_DRAW);

    unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Create FrameBuffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Setup FrameBuffer Texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, view_width, view_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


    //Configuring Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Configure Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Wireframe rendering
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*
    * Render Loop
    * Swap color buffer then polls for events like keyboard or mouse input
    */

    glEnable(GL_BLEND);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Straight Opacity
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //Premult opacity
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glBlendColor(0.0, 0.0, 0.0, 0.0);
        
    //Sets clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //If you don't want to clear in the render loop for some reason, clear both buffers or else u get epilepsy
    /*glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT);*/
    
    float time;

    // ##### DEBUG
    //glClear(GL_COLOR_BUFFER_BIT);

    //time = glfwGetTime();
    //if (time > glm::two_pi<float>()) {
    //    time -= glm::two_pi<float>();
    //    glfwSetTime(time);
    //}
    ////printf("time: % 1.3f \n", time);
    //shader.use();

    //glUniform1f(timeUniformLocation, time);
    //std::cout << glGetError() << std::endl;

    //glBindVertexArray(VAO);
    ////std::cout << glGetError() << std::endl;

    //glDrawArrays(GL_LINES, 0, lineCount);
    // ##### DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete");
        std::cout << glGetError() << std::endl;
        glfwSetWindowShouldClose(window, true);
    }


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        time = glfwGetTime();
        if (time * speed > glm::two_pi<float>()) {
            time -= glm::two_pi<float>() / speed;
            glfwSetTime(time);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        clearShader.use();
        glUniform1f(opacityUniformLocation, uOpacity);
        glBindBuffer(GL_ARRAY_BUFFER, clearVBO);
        glBindVertexArray(clearVAO);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        lineShader.use();
        glUniform1f(timeUniformLocation, time * speed);
        glUniform1f(aspectUniformLocation, aspect);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBindVertexArray(lineVAO);

        glDrawArrays(GL_LINES, 0, lineCount * 2);

        //std::cout << glGetError() << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        quadShader.use();
        glUniform1f(floorUniformLocation, uFloor);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 140));
    }
    lineShader.free();
    clearShader.free();
    glDeleteFramebuffers(1, &FBO);
    glDeleteVertexArrays(1, &lineVAO);
    glDeleteVertexArrays(1, &clearVAO);
    glDeleteBuffers(1, &lineVBO);
    glDeleteBuffers(1, &clearVBO);

    glfwTerminate();
    return 0;
}



/*
* Resizes viewport when window is resized
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    view_width = width;
    view_height = height;
    glViewport(0, 0, width, height);
    aspect = (float)width / (float)height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

}

/*
* Listening to key events. This is good for stuff like on release
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //Listening for GLFW_RELEASE is like onkeyreleased in game engines
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        uOpacity = fmin(uOpacity + 0.01, 1);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        uOpacity = fmax(uOpacity - 0.01, 0);
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        speed += 1;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        speed -= 1;
    }
    else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        uFloor += 0.01f;
    }
    else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        uFloor -= 0.01f;
    }
    printf("opacity %1.3f \n", uOpacity);
    printf("speed %1.0f \n", speed);
    printf("floor %1.3f \n", uFloor);

}