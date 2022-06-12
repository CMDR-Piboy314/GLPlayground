// Programmed by Piboy314

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>
#include <cglm/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// #define NK_IMPLEMENTATION
// #include <nuklear/nuklear.h>

// Local includes
#include "shader.h"

// Define constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Define variables
int wireframe = 0;
int changeWireframe = 0;
int jAlreadyPressed = 0;

// Vsync is enabled by default - to disable it change this variable to 0
int vsyncEnabled = 1;

// Define functions

// Function to be called when the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// Tell OpenGL to use the full window again, with the new width and height
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	changeWireframe = 0;

	// If the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// Tell GLFW the window should close
		glfwSetWindowShouldClose(window, 1);
	}

	if (!jAlreadyPressed) {
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			changeWireframe = 1;
			jAlreadyPressed = 1;
		}
	}

	else {
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
			jAlreadyPressed = 0;
		}
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
		printf("				 .88888888:.\n				88888888.88888.\n			  .8888888888888888.\n			  888888888888888888\n			  88\' _`88'_  `88888\n			  88 88 88 88  88888\n			  88_88_::_88_:88888\n			  88:::,::,:::::8888\n			  88`:::::::::\'`8888\n			 .88  `::::\'	8:88.\n			8888			`8:888.\n		  .8888\'			 `888888.\n		 .8888:..  .::.  ...:\'8888888:.\n		.8888.'	 :'	 `\'::`88:88888\n	   .8888		\'		 `.888:8888.\n	  888:8		 .		   888:88888\n	.888:88		.:		   888:88888:\n	8888888.	   ::		   88:888888\n	`.::.888.	  ::		  .88888888\n   .::::::.888.	::		 :::`8888\'.:.\n  ::::::::::.888   \'		 .::::::::::::\n  ::::::::::::.8	\'	  .:8::::::::::::.\n .::::::::::::::.		.:888:::::::::::::\n :::::::::::::::88:.__..:88888:::::::::::\'\n  `\'.:::::::::::88888888888.88:::::::::\'\n		`\':::_:' -- \'' -'-' `\':_::::\'`\n");
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	if (vsyncEnabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
	
	struct shaderStruct *myShaderPtr, myShader;
	myShaderPtr = &myShader;

	shaderInit(myShaderPtr, "src/shaders/tri.vert", "src/shaders/tri.frag");

	float vertices[] = {
		//X      Y     Z       X     Y
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top left 
    };

	unsigned int indices[] = {  
		0, 1, 3, // First triangle
		1, 2, 3  // Second triangle
	};

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

	// Ways for the graphics card to manage the data
	// 1. GL_STREAM_DRAW:  Data is set only once and used by the GPU at most a few times.
	// 2. GL_STATIC_DRAW:  Data is set only once and used many times.
	// 3. GL_DYNAMIC_DRAW: Data is changed a lot and used many times.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell OpenGL how to handle the vertex data
	// NOTE: Each vertex attribute takes its data from memory managed by a VBO
	// and which VBO it takes its data from is determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer.
	// Since the previously defined VBO is still bound before calling glVertexAttribPointer vertex attribute 0 is now associated with its vertex data.

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Colour attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load and create texture
	unsigned int texture;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture wrapping and filtering options

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(1);
	unsigned char *data = stbi_load("res/img/container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	else {
		printf("Failed to load image at res/img/container.jpg!\n");
	}

	unsigned int texture2;
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set texture wrapping and filtering options

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *data2 = stbi_load("res/img/Piboy314.png", &width, &height, &nrChannels, 0);

	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data2);
	}

	else {
		printf("Failed to load image at res/img/Piboy314.png!\n");
	}

	// For each sampler tell openGL which texture unit it belongs to (only has to be done once)
	shaderUse(myShaderPtr); 
	shaderSetInt(myShaderPtr, "texture1", 0);
	shaderSetInt(myShaderPtr, "texture2", 1);

	// Initialize variables for framerate counting
	double lastTime = glfwGetTime();
	int frameCount = 0;

	// Program loop
	while (!glfwWindowShouldClose(window)) {
		// Calculate framerate
		double thisTime = glfwGetTime();
		frameCount++;

		// If a second has passed.
		if (thisTime - lastTime >= 1.0) {
			printf("%i FPS\n", frameCount);

			frameCount = 0;
			lastTime = thisTime;
		}

		processInput(window);

		// Clear the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Bind textures on texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shaderUse(myShaderPtr);

		// Create transformations
		mat4 model      = {{1.0f}};
		mat4 view       = {{1.0f}};
		mat4 projection = {{1.0f}};

		glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f});

		// Translating the scene in the reverse direction of where the user wants to move
		glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
		glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f, projection);

		// Retrieve the matrix uniform locations and pass them to the shaders
		GLint modelLoc = glGetUniformLocation(myShaderPtr->shaderID, "model");
		GLint viewLoc = glGetUniformLocation(myShaderPtr->shaderID, "view");
		GLint projectionLoc = glGetUniformLocation(myShaderPtr->shaderID, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
		//shaderSetMat4(myShaderPtr, "projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // Swap the front and back buffers
		glfwPollEvents(); // Check for events (mouse movement, mouse click, keyboard press, keyboard release etc.)
	}

	// Clean up VAO, VBO and EBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Clean up GLFW
	glfwTerminate();

	return 0;
}
