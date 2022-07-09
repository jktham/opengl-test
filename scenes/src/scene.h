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
#include <vector>

#include <stb_image/stb_image.h>
#include "camera.h"

class Scene
{
public:

	virtual std::vector<float> getVertices()
	{
		std::vector<float> vertices(9);
		vertices = {
			 0.0f, 0.0f,  0.0f,
			20.0f, 0.0f, 20.0f,
			20.0f, 0.0f,  0.0f,

			 0.0f, 0.0f,  0.0f,
			20.0f, 0.0f, 20.0f,
			 0.0f, 0.0f, 20.0f
		};
		return vertices;
	}

	virtual unsigned int getShader()
	{
		// vertex shader
		const char* vertexShaderSource;

		std::ifstream vertFile("res/shaders/scene.vs");
		std::string vertString((std::istreambuf_iterator<char>(vertFile)),
			std::istreambuf_iterator<char>());
		vertexShaderSource = vertString.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		const char* fragmentShaderSource;

		std::ifstream fragFile("res/shaders/scene.fs");
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

	virtual unsigned int getVAO()
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		return VAO;
	}

	virtual unsigned int getVBO(std::vector<float> vertices)
	{
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		return VBO;
	}

	virtual void setAttributes()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	virtual void setUniformInt(unsigned int shaderProgram, std::string name, int value)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}

	virtual void setUniformFloat(unsigned int shaderProgram, std::string name, float value)
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}

	virtual void setUniformVec3(unsigned int shaderProgram, std::string name, glm::vec3 value)
	{
		glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2]);
	}

	virtual void setUniformMat4(unsigned int shaderProgram, std::string name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	virtual void render(unsigned int VAO, std::vector<float> vertices, unsigned int shaderProgram, Camera camera, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT, float delta_time)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

		glUseProgram(shaderProgram);
		setUniformMat4(shaderProgram, "model", model);
		setUniformMat4(shaderProgram, "view", view);
		setUniformMat4(shaderProgram, "projection", projection);
		setUniformFloat(shaderProgram, "time", (float)glfwGetTime());

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
	}
};