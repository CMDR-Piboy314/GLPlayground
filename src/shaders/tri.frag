#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

// Texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	// Lerp between both textures (80% Box, 20% Piboy)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}

