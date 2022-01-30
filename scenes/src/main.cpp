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

#include "stb_image.h"
#include "camera.h"
#include "scene.h"
#include "wave.h"
#include "cube.h"

// settings
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

// camera
Camera camera(glm::vec3(10.0f, 5.0f, 30.0f));
float last_x = WINDOW_WIDTH / 2.0f;
float last_y = WINDOW_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f;
float last_frame = 0.0f;
int frame_counter = 0;
float delta_time_sum = 0.0f;
float frame_rate = 0.0f;

// scene
Scene* scene = nullptr;
int scene_index = 0;
std::vector<float> vertices;
unsigned int VAO;
unsigned int VBO;
unsigned int shaderProgram;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

bool draw_fill;

void setScene()
{
	delete scene;

	if (scene_index == 0)
	{
		scene = new Scene;
	}
	else if (scene_index == 1)
	{
		scene = new Wave;
	}
	else if (scene_index == 2)
	{
		scene = new Cube;
	}

	vertices = scene->getVertices();
	VAO = scene->getVAO();
	VBO = scene->getVBO(vertices);
	scene->setAttributes();
	shaderProgram = scene->getShader();
	model = scene->getModel();
	view = scene->getView(camera);
	projection = scene->getProjection(camera, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// callbacks

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float camera_speed = 3.0f * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::RIGHT, delta_time);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.m_speed = SHIFT_SPEED;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.m_speed = SPEED;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		scene_index -= 1;
		if (scene_index > 2)
			scene_index = 0;
		if (scene_index < 0)
			scene_index = 2;
		setScene();
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		scene_index += 1;
		if (scene_index > 2)
			scene_index = 0;
		if (scene_index < 0)
			scene_index = 2;
		setScene();
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		draw_fill = !draw_fill;
		if (draw_fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double pos_x, double pos_y)
{
	if (first_mouse)
	{
		last_x = (float)pos_x;
		last_y = (float)pos_y;
		first_mouse = false;
	}

	float offset_x = (float)pos_x - last_x;
	float offset_y = last_y - (float)pos_y;
	last_x = (float)pos_x;
	last_y = (float)pos_y;

	camera.processMouseMovement(offset_x, offset_y);
}

void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
	camera.processMouseScroll(static_cast<float>(offset_y));
}

int main()
{
	// window setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "wave.cpp", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// scene
	setScene();

	// vertex data
	vertices = scene->getVertices();

	// vertex array object
	VAO = scene->getVAO();

	// vertex buffer object
	VBO = scene->getVBO(vertices);

	// vertex attribute (position)
	scene->setAttributes();

	// shader program
	shaderProgram = scene->getShader();

	// matrices
	model = scene->getModel();
	view = scene->getView(camera);
	projection = scene->getProjection(camera, WINDOW_WIDTH, WINDOW_HEIGHT);

	// opengl settings
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// timing
		float current_frame = (float)glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		if (frame_counter < 59)
		{
			frame_counter += 1;
			delta_time_sum += delta_time;
		}
		else
		{
			frame_rate = 1.0f / delta_time_sum * 60.0f;
			frame_counter = 0;
			delta_time_sum = 0.0f;
		}

		std::cout << "time: " << (float)glfwGetTime() << ", delta: " << delta_time << ", fps: " << frame_rate << "\n";

		// keyboard input
		processInput(window);

		// clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update matrices, set uniforms, draw vertices
		scene->render(VAO, vertices, shaderProgram, camera, WINDOW_WIDTH, WINDOW_HEIGHT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}