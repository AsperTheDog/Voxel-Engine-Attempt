#include "keyboard_movement_controller.hpp"

// std
#include <algorithm>
#include <iostream>
#include <limits>

namespace lve {
	void KeyboardMovementController::moveInPlaneXZ(
		GLFWwindow* window, const float dt, LveGameObject& gameObject, const lve::LveCamera& cam) {

		glm::vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir -= cam.front;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir += cam.front;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += cam.right;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= cam.right;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir -= upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir += upDir;

		if (glfwGetKey(window, keys.printCoords) != GLFW_PRESS && hasPressedR) hasPressedR = false;
		else if (glfwGetKey(window, keys.printCoords) == GLFW_PRESS && !hasPressedR)
		{
			std::cout << gameObject.transform.translation.x << " " << gameObject.transform.translation.y << " " << gameObject.transform.translation.z << "\n";
			hasPressedR = true;
		}

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.translation += (moveSpeed * speedMult) * dt * glm::normalize(moveDir);
		}
	}

	void KeyboardMovementController::updateMouseInput(GLFWwindow* window, lve::LveCamera& cam, const float dt)
	{
		double mouseX = 0.0;
		double mouseY = 0.0;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		mouseOffsetX = mouseX - lastMouseX;
		mouseOffsetY = mouseY - lastMouseY;
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		cam.updateLookVectors(mouseOffsetY, mouseOffsetX, dt, upDir);
	}


	void KeyboardMovementController::updateScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (yoffset < 0)
			speedMult -= std::max(speedMult * REL_RATE, MIN_RATE);
		else if (yoffset > 0)
			speedMult += std::max(speedMult * REL_RATE, MIN_RATE);
		speedMult = std::clamp(speedMult, MIN_SPEED, MAX_SPEED);
		std::cout << speedMult << "\n";
	}
}  // namespace lve