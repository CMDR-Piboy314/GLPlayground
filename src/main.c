// Programmed by Piboy314

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/stb_image.h"

#include "shader.h"

// Define constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Define variables
int wireframe = 0;
int changeWireframe = 0;

float vertices[] = {
    // R     G     B  |  X     Y     Z
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom left
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Top
};

const char *vertexShaderSource;

const char *fragmentShaderSource;

// const char *vertexShaderSource =
// 	"#version 330 core\n"

//     "layout (location = 0) in vec3 aPos;\n"
//     "layout (location = 1) in vec3 aColor;\n"

//     "out vec3 ourColor;\n"

//     "void main() {\n"
//     "   gl_Position = vec4(aPos, 1.0);\n"
//     "   ourColor = aColor;\n"
//     "}\0";

// const char *fragmentShaderSource = 
// 	"#version 330 core\n"

//     "out vec4 FragColor;\n"
//     "in vec3 ourColor;\n"

//     "void main() {\n"
//     "   FragColor = vec4(ourColor, 1.0f);\n"
//     "}\n\0";

// Define functions

// Function to be called when the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// Tell OpenGL to use the full window again, with the new width and height
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	// If the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// Tell GLFW the window should close
		glfwSetWindowShouldClose(window, 1);
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		changeWireframe = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		changeWireframe = 0;
	}

	if (changeWireframe) {
		changeWireframe = 0;

		// If wireframe is 0 (disabled) enable it, vice versa
		if (!wireframe) {
			wireframe = 1;

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		else {
			wireframe = 0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

int main() {
	glfwInit();

	// Tell GLFW we want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Load only modern OpenGL functions, core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	#ifdef __linux__
		printf("                 .88888888:.\n                88888888.88888.\n              .8888888888888888.\n              888888888888888888\n              88\' _`88'_  `88888\n              88 88 88 88  88888\n              88_88_::_88_:88888\n              88:::,::,:::::8888\n              88`:::::::::\'`8888\n             .88  `::::\'    8:88.\n            8888            `8:888.\n          .8888\'             `888888.\n         .8888:..  .::.  ...:\'8888888:.\n        .8888.'     :'     `\'::`88:88888\n       .8888        \'         `.888:8888.\n      888:8         .           888:88888\n    .888:88        .:           888:88888:\n    8888888.       ::           88:888888\n    `.::.888.      ::          .88888888\n   .::::::.888.    ::         :::`8888\'.:.\n  ::::::::::.888   \'         .::::::::::::\n  ::::::::::::.8    \'      .:8::::::::::::.\n .::::::::::::::.        .:888:::::::::::::\n :::::::::::::::88:.__..:88888:::::::::::\'\n  `\'.:::::::::::88888888888.88:::::::::\'\n        `\':::_:' -- \'' -'-' `\':_::::\'`\n");
	#endif

	// Create window object, this object will hold all windowing data and required by most GLFW functions
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLPlayground", NULL, NULL);

	if (window == NULL) {
		printf("Failed to create GLFW window!\n");

		glfwTerminate();
		return -1;
	}

	// Tell GLFW to use the window object as the current context
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");

		return -1;
	}

	// Tell OpenGL to use our entire window, starting from (0, 0) to the full window size in pixels
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Tell GLFW to call the frameBufferSizeCallback function when the window is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	shaderInit("src/shaders/tri.vert", "src/shaders/tri.frag");

	// vertexShaderSource = loadfile("src/shaders/tri.frag");
	// fragmentShaderSource = loadfile("src/shaders/tri.vert");

	// Create VAO + VBO + EBO
	// NOTE: The VAO stores
	// 1. Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
	// 2. Vertex attribute configurations via glVertexAttribPointer.
	// 3. Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.

	// An EBO is a buffer that stores indices that OpenGL uses to decide what vertices to draw (indexed drawing)
	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Ways for the graphics card to manage the data
	// 1. GL_STREAM_DRAW:  Data is set only once and used by the GPU at most a few times.
	// 2. GL_STATIC_DRAW:  Data is set only once and used many times.
	// 3. GL_DYNAMIC_DRAW: Data is changed a lot and used many times.

	// Copy vertex data into buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	unsigned int vertexShader;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the source code of the vertex shader to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	// Check if shader compilation was successful
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

	if (!success) {
		printf("Shader compilation failed! %s\n", infoLog);
	}

	unsigned int fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

	if (!success) {
		printf("Shader compilation failed! %s\n", infoLog);
	}

	// Combine the frag and vert shaders into a single shaderProgram object
	unsigned int shaderProgram;

	shaderProgram = glCreateProgram();

	// Attatch and link
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

	if (!success) {
		printf("shaderProgram failed to link! %s\n", infoLog);
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Tell OpenGL how to handle the vertex data
	// NOTE: Each vertex attribute takes its data from memory managed by a VBO
	// and which VBO it takes its data from is determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer.
	// Since the previously defined VBO is still bound before calling glVertexAttribPointer vertex attribute 0 is now associated with its vertex data.

	// Position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Colour attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO + Registered VBO
	// glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	// Program loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the triangle
		shaderUse();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); // Swap the front and back buffers
		glfwPollEvents(); // Check for events (mouse movement, mouse click, keyboard press, keyboard release etc.)
	}

	// Clean up VAO, VBO and shaderProgram
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Clean up GLFW
	glfwTerminate();

	return 0;
}
