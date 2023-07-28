#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ToggleMovement(GLFWwindow* window);
unsigned int LoadTexture(const std::string& texturePath);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isFirstMouse = true;
float lastX = 400.0f, lastY = 300.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 ambient(0.2f, 0.2f, 0.2f);
glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
glm::vec3 specular(1.0f, 1.0f, 1.0f);
float shininess = 32.0f;

bool isCursorEnable = false;
bool isFlashOn = true;

const std::string vertexShaderPath = "Shaders/shader.vert";
const std::string fragmentShaderPath = "Shaders/shader.frag";
const std::string vertexLightShaderPath = "Shaders/lightshader.vert";
const std::string fragmentLightShaderPath = "Shaders/lightshader.frag";

Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
#pragma region Window

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

#pragma endregion

#pragma region GUI

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags != ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags != ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

#pragma endregion

	Shader shader, lightshader;
	shader.CreateFromFile(vertexShaderPath, fragmentShaderPath);
	lightshader.CreateFromFile(vertexLightShaderPath, fragmentLightShaderPath);

	stbi_set_flip_vertically_on_load(true);
	Texture diffuseMap, specularMap, emissionMap;
	diffuseMap.CreateTexture("Textures/container2.png");
	specularMap.CreateTexture("Textures/container2_specular.png");
	emissionMap.CreateTexture("Textures/matrix.jpg");

#pragma region Vertex

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

#pragma endregion

	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		ProcessInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("shininess", &shininess, 0.0f, 128.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("ambient", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Toggle Movement"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				ToggleMovement(window);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Rendering Process
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightshader.UseShader();

		glm::mat4 view = mainCamera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(mainCamera.FOV), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

		lightshader.SetMat4("view", view);
		lightshader.SetMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightshader.SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		shader.UseShader();

		shader.SetInt("material.diffuse", 0);
		shader.SetInt("material.specular", 1);
		shader.SetInt("material.emission", 2);
		shader.SetFloat("material.shininess", shininess);

		// directional light
		shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		shader.SetVec3("pointLights[0].position", pointLightPositions[0]);
		shader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader.SetFloat("pointLights[0].constant", 1.0f);
		shader.SetFloat("pointLights[0].linear", 0.09f);
		shader.SetFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		shader.SetVec3("pointLights[1].position", pointLightPositions[1]);
		shader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shader.SetFloat("pointLights[1].constant", 1.0f);
		shader.SetFloat("pointLights[1].linear", 0.09f);
		shader.SetFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		shader.SetVec3("pointLights[2].position", pointLightPositions[2]);
		shader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shader.SetFloat("pointLights[2].constant", 1.0f);
		shader.SetFloat("pointLights[2].linear", 0.09f);
		shader.SetFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		shader.SetVec3("pointLights[3].position", pointLightPositions[3]);
		shader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shader.SetFloat("pointLights[3].constant", 1.0f);
		shader.SetFloat("pointLights[3].linear", 0.09f);
		shader.SetFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		shader.SetVec3("spotLight.position", mainCamera.Position);
		shader.SetVec3("spotLight.direction", mainCamera.Front);
		if (isFlashOn)
		{
			shader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			shader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		}
		else
		{
			shader.SetVec3("spotLight.ambient", glm::vec3(0.0f));
			shader.SetVec3("spotLight.diffuse", glm::vec3(0.0f));
			shader.SetVec3("spotLight.specular", glm::vec3(0.0f));
		}
		shader.SetFloat("spotLight.constant", 1.0f);
		shader.SetFloat("spotLight.linear", 0.09f);
		shader.SetFloat("spotLight.quadratic", 0.032f);
		shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		shader.SetVec3("viewPos", mainCamera.Position);
		shader.SetFloat("time", glfwGetTime());

		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		diffuseMap.ActiveTexture(0);
		specularMap.ActiveTexture(1);
		emissionMap.ActiveTexture(2);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Poll Events & Swap Buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Welcome Back OpenGL!!" << std::endl;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.ClearShader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		ToggleMovement(window);
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		isFlashOn = !isFlashOn;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (isFirstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		isFirstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (isCursorEnable) return;
	mainCamera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	mainCamera.ProcessMouseScroll(yOffset);
}

void ToggleMovement(GLFWwindow* window)
{
	if(isCursorEnable)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	isCursorEnable = !isCursorEnable;
}

unsigned int LoadTexture(const std::string& texturePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAIL_LOAD_TEXTURE" << std::endl;
		return 0;
	}

	stbi_image_free(data);
	return textureID;
}