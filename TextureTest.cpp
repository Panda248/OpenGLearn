//Learning about rendering textures

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
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

	/*
	* Instantiate shader program
	*/
	Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	shader.use();

	/*
	* Configuring Texture wrapping
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	/*
	* Configure Texture Filtering
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	* Loading image using stb_image
	*/

	//width, height, number of color channels
	int width, height, nrChannels;
	unsigned char* data = stbi_load("7a9.jpg", &width, &height, &nrChannels);
		
	/*
	* Generate texture
	*/
	unsigned int texture;
	glGenTextures(1, &texture);


	//Wireframe rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*
	* Render Loop
	* Swap color buffer then polls for events like keyboard or mouse input
	*/

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		//glClear(GL_COLOR_BUFFER_BIT);


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