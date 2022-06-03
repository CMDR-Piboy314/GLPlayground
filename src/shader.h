#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

int ID;

const char* loadFile(char* filepath) {
	FILE *file = fopen(filepath, "r");
	char *filecontents;

	size_t n = 0;
	int c;

	if (file == NULL) {
		printf("Failed to load file at: %s!\n", filepath);
	}

	filecontents = malloc(1000);

	while ((c = fgetc(file)) != EOF) {
		filecontents[n++] = (char) c;
	}

	// Terminate with null character
	filecontents[n] = '\0';

	return filecontents;
}

// Utility function for checking shader compilation/linking errors.
void shaderCheckCompileErrors(unsigned int shader, char* type) {
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
		}
	}

	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR: PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
		}
	}
}

// Constructor generates the shader on the fly
void shaderInit(const char* vertexPath, const char* fragmentPath) {
	// 1. Get the source code from the files into strings
	const char* vShaderCode = loadFile("src/shaders/tri.vert");
	const char * fShaderCode = loadFile("src/shaders/tri.frag");

	// 2. Compile shaders
	unsigned int vertex, fragment;

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	shaderCheckCompileErrors(vertex, "VERTEX");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	shaderCheckCompileErrors(fragment, "FRAGMENT");

	// Shader Program
	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);
	shaderCheckCompileErrors(ID, "PROGRAM");

	// Delete the shaders as they're linked into the program now and no longer needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Activate the shader
void shaderUse() {
	glUseProgram(ID);
}

// Utility uniform functions
// void setBool(const char* name, bool value) {
//     glUniform1i(glGetUniformLocation(ID, &name), (int)value);
// }

void shaderSetInt(const char *name, int value) {
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void shaderSetFloat(const char *name, float value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

#endif