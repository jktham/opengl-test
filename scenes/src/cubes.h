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

class Cubes : public Scene
{
public:

	std::vector<float> getVertices()
	{
		float cube_width = 1.0f;
		std::vector<float> vertices(6 * 18);

		vertices = {
			-cube_width, -cube_width, -cube_width,
			 cube_width, -cube_width, -cube_width,
			 cube_width,  cube_width, -cube_width,
			 cube_width,  cube_width, -cube_width,
			-cube_width,  cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,

			-cube_width, -cube_width,  cube_width,
			 cube_width, -cube_width,  cube_width,
			 cube_width,  cube_width,  cube_width,
			 cube_width,  cube_width,  cube_width,
			-cube_width,  cube_width,  cube_width,
			-cube_width, -cube_width,  cube_width,

			-cube_width,  cube_width,  cube_width,
			-cube_width,  cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,
			-cube_width, -cube_width, -cube_width,
			-cube_width, -cube_width,  cube_width,
			-cube_width,  cube_width,  cube_width,

			 cube_width,  cube_width,  cube_width,
			 cube_width,  cube_width, -cube_width,
			 cube_width, -cube_width, -cube_width,
			 cube_width, -cube_width, -cube_width,
			 cube_width, -cube_width,  cube_width,
			 cube_width,  cube_width,  cube_width,

			-cube_width, -cube_width, -cube_width,
			 cube_width, -cube_width, -cube_width,
			 cube_width, -cube_width,  cube_width,
			 cube_width, -cube_width,  cube_width,
			-cube_width, -cube_width,  cube_width,
			-cube_width, -cube_width, -cube_width,

			-cube_width,  cube_width, -cube_width,
			 cube_width,  cube_width, -cube_width,
			 cube_width,  cube_width,  cube_width,
			 cube_width,  cube_width,  cube_width,
			-cube_width,  cube_width,  cube_width,
			-cube_width,  cube_width, -cube_width
		};
		return vertices;
	}

	unsigned int getShader()
	{
		// vertex shader
		const char* vertexShaderSource;

		std::ifstream vertFile("res/shaders/cubes.vs");
		std::string vertString((std::istreambuf_iterator<char>(vertFile)),
			std::istreambuf_iterator<char>());
		vertexShaderSource = vertString.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		const char* fragmentShaderSource;

		std::ifstream fragFile("res/shaders/cubes.fs");
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

	bool generated = false;
	float cube_area = 400.0f;
	static const int cube_amount = 10000;
	glm::vec3 cube_positions[cube_amount]{};
	unsigned int instanceVBO;

	void render(unsigned int VAO, std::vector<float> vertices, unsigned int shaderProgram, Camera camera, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT, float delta_time)
	{
		if (!generated)
		{
			int seed = (int)(100 * (float)glfwGetTime());
			srand((unsigned)seed);

			for (int i = 0; i < cube_amount; i++)
			{
				cube_positions[i] = glm::vec3((float)std::rand() / RAND_MAX * cube_area - cube_area / 2.0f, (float)std::rand() / RAND_MAX * cube_area - cube_area / 2.0f, (float)std::rand() / RAND_MAX * cube_area - cube_area / 2.0f);
			}

			glGenBuffers(1, &instanceVBO);
			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube_amount, &cube_positions[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(1, 1);

			generated = true;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

		glUseProgram(shaderProgram);
		setUniformMat4(shaderProgram, "model", model);
		setUniformMat4(shaderProgram, "view", view);
		setUniformMat4(shaderProgram, "projection", projection);
		setUniformFloat(shaderProgram, "time", (float)glfwGetTime());


		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)vertices.size(), cube_amount);
	}
};