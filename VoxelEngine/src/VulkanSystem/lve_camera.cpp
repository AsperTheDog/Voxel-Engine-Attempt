#include "lve_camera.hpp"

// std
#include <cassert>
#include <iostream>
#include <limits>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RIGHT_HANDED

namespace lve {

	void LveCamera::setOrthographicProjection(
		float left, float right, float top, float bottom, float near, float far) {
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = 2.f / (right - left);
		projectionMatrix[1][1] = 2.f / (bottom - top);
		projectionMatrix[2][2] = 1.f / (far - near);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -near / (far - near);
	}

	void LveCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = far / (far - near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(far * near) / (far - near);
	}

	void LveCamera::setViewMatrix(const glm::vec3 position) {
		viewMatrix = glm::lookAt(position, position + front, up);
	}

	void LveCamera::updateLookVectors(
		const double mouseOffsetY,
		const double mouseOffsetX,
		const float dt,
		const glm::vec3 upDir)
	{

		pitch -= static_cast<float>(mouseOffsetY) * lookSpeed * dt;
		yaw -= static_cast<float>(mouseOffsetX) * lookSpeed * dt;

		if (pitch > 80.0f)
			pitch = 80.0f;
		else if (pitch < -80.0f)
			pitch = -80.0f;

		if (yaw > 360.0f || yaw < -360.0f)
			yaw = 0.0f;


		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontMovement.x = cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontMovement.z = sin(glm::radians(yaw));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, upDir));
		up = glm::normalize(glm::cross(right, front));
	}

}  // namespace lve
