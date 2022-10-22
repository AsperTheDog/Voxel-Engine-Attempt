#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"
#include "lve_window.hpp"

#define MIN_SPEED 0.01f
#define MAX_SPEED 10.f
#define MIN_RATE 0.03f
#define REL_RATE 0.05f

namespace lve {
	class KeyboardMovementController {
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_SPACE;
			int moveDown = GLFW_KEY_LEFT_SHIFT;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
			int printCoords = GLFW_KEY_R;
		};

		void updateMouseInput(GLFWwindow* window);

		void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject, const lve::LveCamera& cam);
		void updateMouseInput(GLFWwindow* window, lve::LveCamera& cam, float dt);

		KeyMappings keys{};
		float moveSpeed{ 100.f };
		const glm::vec3 upDir{ 0.f, 1.f, 0.f };

		double mouseOffsetX;
		double mouseOffsetY;

		static void updateScroll(GLFWwindow* window, double xoffset, double yoffset);
		inline static float speedMult = 1.0f;
	private:

		double lastMouseX = 0.0;
		double lastMouseY = 0.0;
		bool hasPressedR = false;
	};
}  // namespace lve