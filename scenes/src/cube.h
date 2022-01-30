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

#include "stb_image.h"
#include "camera.h"
#include "scene.h"

class Cube : public Scene
{
public:

	std::vector<float> getVertices()
	{
		float cube_width = 8.0f;
		std::vector<float> vertices(6 * 18);

		vertices = {
			-cube_width, -cube_width, -cube_width,
			cube_width, -cube_width, -cube_width,
			cube_width, cube_width, -cube_width,
			cube_width, cube_width, -cube_width,
			-cube_width, cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,

			-cube_width, -cube_width, cube_width,
			cube_width, -cube_width, cube_width,
			cube_width, cube_width, cube_width,
			cube_width, cube_width, cube_width,
			-cube_width, cube_width, cube_width,
			-cube_width, -cube_width, cube_width,

			-cube_width, cube_width, cube_width,
			-cube_width, cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,
			-cube_width, -cube_width, cube_width,
			-cube_width, cube_width, cube_width,

			cube_width, cube_width, cube_width,
			cube_width, cube_width, -cube_width,
			cube_width, -cube_width, -cube_width,
			cube_width, -cube_width, -cube_width,
			cube_width, -cube_width, cube_width,
			cube_width, cube_width, cube_width,

			-cube_width, -cube_width, -cube_width,
			cube_width, -cube_width, -cube_width,
			cube_width, -cube_width, cube_width,
			cube_width, -cube_width, cube_width,
			-cube_width, -cube_width, cube_width,
			-cube_width, -cube_width, -cube_width,

			-cube_width, cube_width, -cube_width,
			cube_width, cube_width, -cube_width,
			cube_width, cube_width, cube_width,
			cube_width, cube_width, cube_width,
			-cube_width, cube_width, cube_width,
			-cube_width, cube_width, -cube_width
		};
		return vertices;
	}

	unsigned int getShader()
	{
		// vertex shader
		const char* vertexShaderSource;

		std::ifstream vertFile("src/cube.vs");
		std::string vertString((std::istreambuf_iterator<char>(vertFile)),
			std::istreambuf_iterator<char>());
		vertexShaderSource = vertString.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		const char* fragmentShaderSource;

		std::ifstream fragFile("src/cube.fs");
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

	float cube_rotation = 0.0f;
	float cube_rotation_speed = 0.02f;
	glm::mat4 model = glm::mat4(1.0f);

	virtual glm::mat4 getModel()
	{
		cube_rotation += cube_rotation_speed;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 10.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

		model = glm::rotate(model, cube_rotation, glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)));

		return model;
	}
};