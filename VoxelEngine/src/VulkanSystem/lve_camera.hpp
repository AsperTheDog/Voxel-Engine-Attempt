#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "lve_window.hpp"

namespace lve {

	class LveCamera {
	public:
		void setOrthographicProjection(
			float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		void setViewMatrix(glm::vec3 position);

		const glm::mat4& getProjection() const { return projectionMatrix; }
		const glm::mat4& getView() const { return viewMatrix; }
		void updateLookVectors(double mouseOffsetY, double mouseOffsetX, float dt, glm::vec3 upDir);

		glm::vec3 right = { 0.f, 0.f, 0.f };
		glm::vec3 front = { 0.f, 0.f, -1.f };

	private:

		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.f };
		float lookSpeed{ 4.f };

		float pitch = 0.0f;
		float yaw = -90.0f;
		float roll = 0.0f;

		glm::vec3 frontMovement = {};
		glm::vec3 up = {};
	};
}  // namespace lve