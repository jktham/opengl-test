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

class Sphere : public Scene
{
public:

	std::vector<float> getVertices()
	{
		float width = 10.0f;
		float height = 13.0f;
		std::vector<float> sides(9 * 8);

		sides = {
			-width,  0.0f, -width,
			 0.0f, -height,  0.0f,
			 width,  0.0f, -width,

			 width,  0.0f, -width,
			 0.0f, -height,  0.0f,
			 width,  0.0f,  width,

			 width,  0.0f,  width,
			 0.0f, -height,  0.0f,
			-width,  0.0f,  width,

			-width,  0.0f,  width,
			 0.0f, -height,  0.0f,
			-width,  0.0f, -width,

			-width,  0.0f, -width,
			 0.0f,  height,  0.0f,
			 width,  0.0f, -width,

			 width,  0.0f, -width,
			 0.0f,  height,  0.0f,
			 width,  0.0f,  width,

			 width,  0.0f,  width,
			 0.0f,  height,  0.0f,
			-width,  0.0f,  width,

			-width,  0.0f,  width,
			 0.0f,  height,  0.0f,
			-width,  0.0f, -width,
		};

		std::vector<float> vertices(9 * 8 * 4);
		vertices = subdivide(sides);

		return vertices;
	}

	std::vector<float> subdivide(std::vector<float> sides)
	{
		std::vector<float> vertices(9 * 8 * 4);

		for (int i = 0; i < vertices.size(); i++)
		{
		}

		vertices = sides;

		return vertices;
	}

	unsigned int getShader()
	{
		// vertex shader
		const char* vertexShaderSource;

		std::ifstream vertFile("res/shaders/sphere.vs");
		std::string vertString((std::istreambuf_iterator<char>(vertFile)),
			std::istreambuf_iterator<char>());
		vertexShaderSource = vertString.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		const char* fragmentShaderSource;

		std::ifstream fragFile("res/shaders/sphere.fs");
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

	virtual void render(unsigned int VAO, std::vector<float> vertices, unsigned int shaderProgram, Camera camera, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT, float delta_time)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 10.0f));

		glUseProgram(shaderProgram);
		setUniformMat4(shaderProgram, "model", model);
		setUniformMat4(shaderProgram, "view", view);
		setUniformMat4(shaderProgram, "projection", projection);
		setUniformFloat(shaderProgram, "time", (float)glfwGetTime());

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
	}
};