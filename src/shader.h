#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include "../lib/glad/include/glad/glad.h"

struct shaderStruct {
	int shaderID;
};

const char* loadFile(const char* filepath) {
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

	if (!strcmp("PROGRAM", type)) {
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
void shaderInit(struct shaderStruct* shaderStructInstance, const char* vertexPath, const char* fragmentPath) {
	// 1. Get the source code from the files into strings
	const char* vShaderCode = loadFile(vertexPath);
	const char * fShaderCode = loadFile(fragmentPath);

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
	shaderStructInstance->shaderID = glCreateProgram();

	glAttachShader(shaderStructInstance->shaderID, vertex);
	glAttachShader(shaderStructInstance->shaderID, fragment);

	glLinkProgram(shaderStructInstance->shaderID);
	shaderCheckCompileErrors(shaderStructInstance->shaderID, "PROGRAM");

	// Delete the shaders as they're linked into the program now and no longer needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Activate the shader
void shaderUse(struct shaderStruct* shaderStructInstance) {
	glUseProgram(shaderStructInstance->shaderID);
}

// Utility uniform functions
void shaderSetVec2(struct shaderStruct* shaderStructInstance, const char *name, const vec2 value) { 
	glUniform2fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, &value[0]); 
}


void shaderSetVec3(struct shaderStruct* shaderStructInstance, const char *name, const vec3 value) { 
	glUniform3fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, &value[0]); 
}


void shaderSetVec4(struct shaderStruct* shaderStructInstance, const char *name, const vec4 value) { 
	glUniform4fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, &value[0]); 
}


void shaderSetMat2(struct shaderStruct* shaderStructInstance, const char *name, const mat2 mat) {
	glUniformMatrix2fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, GL_FALSE, &mat[0][0]);
}

void shaderSetMat3(struct shaderStruct* shaderStructInstance, const char *name, const mat3 mat) {
	glUniformMatrix3fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, GL_FALSE, &mat[0][0]);
}

void shaderSetMat4(struct shaderStruct* shaderStructInstance, const char *name, const mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(shaderStructInstance->shaderID, name), 1, GL_FALSE, &mat[0][0]);
}

void shaderSetInt(struct shaderStruct* shaderStructInstance, const char *name, int value) {
	glUniform1i(glGetUniformLocation(shaderStructInstance->shaderID, name), value);
}

void shaderSetFloat(struct shaderStruct* shaderStructInstance, const char *name, float value) {
	glUniform1f(glGetUniformLocation(shaderStructInstance->shaderID, name), value);
}

#endif
