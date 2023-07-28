#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float C_YAW = -90.f;
const float C_PITCH = 0.0f;
const float C_SPEED = 2.5f;
const float C_SENSITIVITY = 0.1f;
const float C_FOV = 45.0f;

class Camera
{
public:
	float FOV;
	float Speed;
	float Sensitivity;

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;

	Camera(const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), const float yaw = C_YAW, const float pitch = C_PITCH)
		: Position(pos), Up(up), m_yaw(yaw), m_pitch(pitch), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Speed(C_SPEED), Sensitivity(C_SENSITIVITY), FOV(C_FOV)
	{
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix();
	
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);

	~Camera();
private:
	float m_yaw;
	float m_pitch;	

	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		front.y = sin(glm::radians(m_pitch));
		front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

