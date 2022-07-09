#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

#include <stb_image/stb_image.h>
#include "camera.h"
#include "scene.h"

class Wave : public Scene
{
public:

	std::vector<float> getVertices()
	{
		const int plane_width = 40;
		const int plane_height = 40;
		std::vector<float> vertices(plane_width * plane_height * 18);

		float plane_spacing = 0.5f;
		float y = 0.0f;

		int i = 0;
		for (float x = 0; x < plane_width; x++) {
			for (float z = 0; z < plane_height; z++) {
				vertices[i++] = x * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = z * plane_spacing;

				vertices[i++] = x * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = (z + 1) * plane_spacing;

				vertices[i++] = (x + 1) * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = (z + 1) * plane_spacing;

				vertices[i++] = x * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = z * plane_spacing;

				vertices[i++] = (x + 1) * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = z * plane_spacing;

				vertices[i++] = (x + 1) * plane_spacing;
				vertices[i++] = y * plane_spacing;
				vertices[i++] = (z + 1) * plane_spacing;
			}
		}
		return vertices;
	}

	unsigned int getShader()
	{
		// vertex shader
		const char* vertexShaderSource;

		std::ifstream vertFile("res/shaders/wave.vs");
		std::string vertString((std::istreambuf_iterator<char>(vertFile)),
			std::istreambuf_iterator<char>());
		vertexShaderSource = vertString.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		const char* fragmentShaderSource;

		std::ifstream fragFile("res/shaders/wave.fs");
		std::string fragString((std::istreambuf_iterator<char>(fragFile)),
			std::istreambuf_iterator<char>());
		fragmentShaderSource = fragString.c_str();

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// shader program
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}
};