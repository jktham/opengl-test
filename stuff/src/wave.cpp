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
#include "shader.h"
#include "camera.h"

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

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

	// vertex data
	const int plane_width = 40;
	const int plane_height = 40;
	float vertices[plane_width * plane_height * 18]{};

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

	// vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex attribute (position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// shader program
	Shader shader("src/wave.vs", "src/wave.fs");

	// matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

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

		// update matrices
		model = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.m_fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		// set uniforms
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setFloat("time", current_frame);

		// draw vertices
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

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