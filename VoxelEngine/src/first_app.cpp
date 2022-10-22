#include "first_app.hpp"

#include "VulkanSystem/keyboard_movement_controller.hpp"
#include "VulkanSystem/lve_buffer.hpp"
#include "VulkanSystem/lve_camera.hpp"
#include "VulkanSystem/systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

#include "VoxelParser/VoxelModelManager.h"

glm::vec2 distancesFar[] = {
	{  0,   0}, {  1,   0}, { -1,   0}, { -1,  -1},
	{  0,  -1}, {  2,   0}, {  1,   1}, {  0,   1},
	{ -1,   1}, { -2,   1}, { -2,   0}, { -2,  -1},
	{ -2,  -2}, { -1,  -2}, {  0,  -2}, {  1,  -2},
	{  1,  -1}, {  3,   0}, {  2,   1}, {  2,   2},
	{  1,   2}, {  0,   2}, { -1,   2}, { -2,   2},
	{ -3,   2}, { -3,   1}, { -3,   0}, { -3,  -1},
	{ -3,  -2}, { -3,  -3}, { -2,  -3}, { -1,  -3},
	{  0,  -3}, {  1,  -3}, {  2,  -3}, {  2,  -2},
	{  2,  -1}, {  4,   0}, {  3,   1}, {  3,   2},
	{  2,   3}, {  1,   3}, {  0,   3}, { -1,   3},
	{ -2,   3}, { -3,   3}, { -4,   2}, { -4,   1},
	{ -4,   0}, { -4,  -1}, { -4,  -2}, { -4,  -3},
	{ -3,  -4}, { -2,  -4}, { -1,  -4}, {  0,  -4},
	{  1,  -4}, {  2,  -4}, {  3,  -3}, {  3,  -2},
	{  3,  -1}, {  5,   0}, {  4,   1}, {  4,   2},
	{  3,   3}, {  2,   4}, {  1,   4}, {  0,   4},
	{ -1,   4}, { -2,   4}, { -3,   4}, { -4,   3},
	{ -5,   2}, { -5,   1}, { -5,   0}, { -5,  -1},
	{ -5,  -2}, { -5,  -3}, { -4,  -4}, { -3,  -5},
	{ -2,  -5}, { -1,  -5}, {  0,  -5}, {  1,  -5},
	{  2,  -5}, {  3,  -4}, {  4,  -3}, {  4,  -2},
	{  4,  -1}, {  6,   0}, {  5,   1}, {  5,   2},
	{  5,   3}, {  4,   3}, {  4,   4}, {  3,   4},
	{  3,   5}, {  2,   5}, {  1,   5}, {  0,   5},
	{ -1,   5}, { -2,   5}, { -3,   5}, { -4,   5},
	{ -4,   4}, { -5,   4}, { -5,   3}, { -6,   3},
	{ -6,   2}, { -6,   1}, { -6,   0}, { -6,  -1},
	{ -6,  -2}, { -6,  -3}, { -6,  -4}, { -5,  -4},
	{ -5,  -5}, { -4,  -5}, { -4,  -6}, { -3,  -6},
	{ -2,  -6}, { -1,  -6}, {  0,  -6}, {  1,  -6},
	{  2,  -6}, {  3,  -6}, {  3,  -5}, {  4,  -5},
	{  4,  -4}, {  5,  -4}, {  5,  -3}, {  5,  -2},
	{  5,  -1}, {  7,   0}, {  6,   1}, {  6,   2},
	{  6,   3}, {  5,   4}, {  4,   5}, {  3,   6},
	{  2,   6}, {  1,   6}, {  0,   6}, { -1,   6},
	{ -2,   6}, { -3,   6}, { -4,   6}, { -5,   5},
	{ -6,   4}, { -7,   3}, { -7,   2}, { -7,   1},
	{ -7,   0}, { -7,  -1}, { -7,  -2}, { -7,  -3},
	{ -7,  -4}, { -6,  -5}, { -5,  -6}, { -4,  -7},
	{ -3,  -7}, { -2,  -7}, { -1,  -7}, {  0,  -7},
	{  1,  -7}, {  2,  -7}, {  3,  -7}, {  4,  -6},
	{  5,  -5}, {  6,  -4}, {  6,  -3}, {  6,  -2},
	{  6,  -1}, {  8,   0}, {  7,   1}, {  7,   2},
	{  7,   3}, {  6,   4}, {  6,   5}, {  5,   5},
	{  5,   6}, {  4,   6}, {  3,   7}, {  2,   7},
	{  1,   7}, {  0,   7}, { -1,   7}, { -2,   7},
	{ -3,   7}, { -4,   7}, { -5,   6}, { -6,   6},
	{ -6,   5}, { -7,   5}, { -7,   4}, { -8,   3},
	{ -8,   2}, { -8,   1}, { -8,   0}, { -8,  -1},
	{ -8,  -2}, { -8,  -3}, { -8,  -4}, { -7,  -5},
	{ -7,  -6}, { -6,  -6}, { -6,  -7}, { -5,  -7},
	{ -4,  -8}, { -3,  -8}, { -2,  -8}, { -1,  -8},
	{  0,  -8}, {  1,  -8}, {  2,  -8}, {  3,  -8},
	{  4,  -7}, {  5,  -7}, {  5,  -6}, {  6,  -6},
	{  6,  -5}, {  7,  -4}, {  7,  -3}, {  7,  -2},
	{  7,  -1}, {  9,   0}, {  8,   1}, {  8,   2},
	{  8,   3}, {  8,   4}, {  7,   4}, {  7,   5},
	{  6,   6}, {  5,   7}, {  4,   7}, {  4,   8},
	{  3,   8}, {  2,   8}, {  1,   8}, {  0,   8},
	{ -1,   8}, { -2,   8}, { -3,   8}, { -4,   8},
	{ -5,   8}, { -5,   7}, { -6,   7}, { -7,   6},
	{ -8,   5}, { -8,   4}, { -9,   4}, { -9,   3},
	{ -9,   2}, { -9,   1}, { -9,   0}, { -9,  -1},
	{ -9,  -2}, { -9,  -3}, { -9,  -4}, { -9,  -5},
	{ -8,  -5}, { -8,  -6}, { -7,  -7}, { -6,  -8},
	{ -5,  -8}, { -5,  -9}, { -4,  -9}, { -3,  -9},
	{ -2,  -9}, { -1,  -9}, {  0,  -9}, {  1,  -9},
	{  2,  -9}, {  3,  -9}, {  4,  -9}, {  4,  -8},
	{  5,  -8}, {  6,  -7}, {  7,  -6}, {  7,  -5},
	{  8,  -5}, {  8,  -4}, {  8,  -3}, {  8,  -2},
	{  8,  -1}, { 10,   0}, {  9,   1}, {  9,   2},
	{  9,   3}, {  9,   4}, {  8,   5}, {  7,   6},
	{  7,   7}, {  6,   7}, {  5,   8}, {  4,   9},
	{  3,   9}, {  2,   9}, {  1,   9}, {  0,   9},
	{ -1,   9}, { -2,   9}, { -3,   9}, { -4,   9},
	{ -5,   9}, { -6,   8}, { -7,   7}, { -8,   7},
	{ -8,   6}, { -9,   5}, {-10,   4}, {-10,   3},
	{-10,   2}, {-10,   1}, {-10,   0}, {-10,  -1},
	{-10,  -2}, {-10,  -3}, {-10,  -4}, {-10,  -5},
	{ -9,  -6}, { -8,  -7}, { -8,  -8}, { -7,  -8},
	{ -6,  -9}, { -5, -10}, { -4, -10}, { -3, -10},
	{ -2, -10}, { -1, -10}, {  0, -10}, {  1, -10},
	{  2, -10}, {  3, -10}, {  4, -10}, {  5,  -9},
	{  6,  -8}, {  7,  -8}, {  7,  -7}, {  8,  -6},
	{  9,  -5}, {  9,  -4}, {  9,  -3}, {  9,  -2},
	{  9,  -1}, { 11,   0}, { 10,   1}, { 10,   2},
	{ 10,   3}, { 10,   4}, {  9,   5}, {  9,   6},
	{  8,   6}, {  8,   7}, {  7,   8}, {  6,   8},
	{  6,   9}, {  5,   9}, {  4,  10}, {  3,  10},
	{  2,  10}, {  1,  10}, {  0,  10}, { -1,  10},
	{ -2,  10}, { -3,  10}, { -4,  10}, { -5,  10},
	{ -6,   9}, { -7,   9}, { -7,   8}, { -8,   8},
	{ -9,   7}, { -9,   6}, {-10,   6}, {-10,   5},
	{-11,   4}, {-11,   3}, {-11,   2}, {-11,   1},
	{-11,   0}, {-11,  -1}, {-11,  -2}, {-11,  -3},
	{-11,  -4}, {-11,  -5}, {-10,  -6}, {-10,  -7},
	{ -9,  -7}, { -9,  -8}, { -8,  -9}, { -7,  -9},
	{ -7, -10}, { -6, -10}, { -5, -11}, { -4, -11},
	{ -3, -11}, { -2, -11}, { -1, -11}, {  0, -11},
	{  1, -11}, {  2, -11}, {  3, -11}, {  4, -11},
	{  5, -10}, {  6, -10}, {  6,  -9}, {  7,  -9},
	{  8,  -8}, {  8,  -7}, {  9,  -7}, {  9,  -6},
	{ 10,  -5}, { 10,  -4}, { 10,  -3}, { 10,  -2},
	{ 10,  -1}, { 12,   0}, { 11,   1}, { 11,   2},
	{ 11,   3}, { 11,   4}, { 10,   5}, { 10,   6},
	{  9,   7}, {  8,   8}, {  7,   9}, {  6,  10},
	{  5,  10}, {  4,  11}, {  3,  11}, {  2,  11},
	{  1,  11}, {  0,  11}, { -1,  11}, { -2,  11},
	{ -3,  11}, { -4,  11}, { -5,  11}, { -6,  10},
	{ -7,  10}, { -8,   9}, { -9,   8}, {-10,   7},
	{-11,   6}, {-11,   5}, {-12,   4}, {-12,   3},
	{-12,   2}, {-12,   1}, {-12,   0}, {-12,  -1},
	{-12,  -2}, {-12,  -3}, {-12,  -4}, {-12,  -5},
	{-11,  -6}, {-11,  -7}, {-10,  -8}, { -9,  -9},
	{ -8, -10}, { -7, -11}, { -6, -11}, { -5, -12},
	{ -4, -12}, { -3, -12}, { -2, -12}, { -1, -12},
	{  0, -12}, {  1, -12}, {  2, -12}, {  3, -12},
	{  4, -12}, {  5, -11}, {  6, -11}, {  7, -10},
	{  8,  -9}, {  9,  -8}, { 10,  -7}, { 10,  -6},
	{ 11,  -5}, { 11,  -4}, { 11,  -3}, { 11,  -2},
	{ 11,  -1}, { 13,   0}, { 12,   1}, { 12,   2},
	{ 12,   3}, { 12,   4}, { 11,   5}, { 11,   6},
	{ 10,   7}, { 10,   8}, {  9,   8}, {  9,   9},
	{  8,   9}, {  8,  10}, {  7,  10}, {  6,  11},
	{  5,  11}, {  4,  12}, {  3,  12}, {  2,  12},
	{  1,  12}, {  0,  12}, { -1,  12}, { -2,  12},
	{ -3,  12}, { -4,  12}, { -5,  12}, { -6,  11},
	{ -7,  11}, { -8,  10}, { -9,  10}, { -9,   9},
	{-10,   9}, {-10,   8}, {-11,   8}, {-11,   7},
	{-12,   6}, {-12,   5}, {-13,   4}, {-13,   3},
	{-13,   2}, {-13,   1}, {-13,   0}, {-13,  -1},
	{-13,  -2}, {-13,  -3}, {-13,  -4}, {-13,  -5},
	{-12,  -6}, {-12,  -7}, {-11,  -8}, {-11,  -9},
	{-10,  -9}, {-10, -10}, { -9, -10}, { -9, -11},
	{ -8, -11}, { -7, -12}, { -6, -12}, { -5, -13},
	{ -4, -13}, { -3, -13}, { -2, -13}, { -1, -13},
	{  0, -13}, {  1, -13}, {  2, -13}, {  3, -13},
	{  4, -13}, {  5, -12}, {  6, -12}, {  7, -11},
	{  8, -11}, {  8, -10}, {  9, -10}, {  9,  -9},
	{ 10,  -9}, { 10,  -8}, { 11,  -7}, { 11,  -6},
	{ 12,  -5}, { 12,  -4}, { 12,  -3}, { 12,  -2},
	{ 12,  -1}, { 14,   0}, { 13,   1}, { 13,   2},
	{ 13,   3}, { 13,   4}, { 13,   5}, { 12,   5},
	{ 12,   6}, { 12,   7}, { 11,   7}, { 11,   8},
	{ 10,   9}, {  9,  10}, {  8,  11}, {  7,  11},
	{  7,  12}, {  6,  12}, {  5,  12}, {  5,  13},
	{  4,  13}, {  3,  13}, {  2,  13}, {  1,  13},
	{  0,  13}, { -1,  13}, { -2,  13}, { -3,  13},
	{ -4,  13}, { -5,  13}, { -6,  13}, { -6,  12},
	{ -7,  12}, { -8,  12}, { -8,  11}, { -9,  11},
	{-10,  10}, {-11,   9}, {-12,   8}, {-12,   7},
	{-13,   7}, {-13,   6}, {-13,   5}, {-14,   5},
	{-14,   4}, {-14,   3}, {-14,   2}, {-14,   1},
	{-14,   0}, {-14,  -1}, {-14,  -2}, {-14,  -3},
	{-14,  -4}, {-14,  -5}, {-14,  -6}, {-13,  -6},
	{-13,  -7}, {-13,  -8}, {-12,  -8}, {-12,  -9},
	{-11, -10}, {-10, -11}, { -9, -12}, { -8, -12},
	{ -8, -13}, { -7, -13}, { -6, -13}, { -6, -14},
	{ -5, -14}, { -4, -14}, { -3, -14}, { -2, -14},
	{ -1, -14}, {  0, -14}, {  1, -14}, {  2, -14},
	{  3, -14}, {  4, -14}, {  5, -14}, {  5, -13},
	{  6, -13}, {  7, -13}, {  7, -12}, {  8, -12},
	{  9, -11}, { 10, -10}, { 11,  -9}, { 11,  -8},
	{ 12,  -8}, { 12,  -7}, { 12,  -6}, { 13,  -6},
	{ 13,  -5}, { 13,  -4}, { 13,  -3}, { 13,  -2},
	{ 13,  -1}, { 15,   0}, { 14,   1}, { 14,   2},
	{ 14,   3}, { 14,   4}, { 14,   5}, { 13,   6},
	{ 13,   7}, { 12,   8}, { 11,   9}, { 11,  10},
	{ 10,  10}, { 10,  11}, {  9,  11}, {  8,  12},
	{  7,  13}, {  6,  13}, {  5,  14}, {  4,  14},
	{  3,  14}, {  2,  14}, {  1,  14}, {  0,  14},
	{ -1,  14}, { -2,  14}, { -3,  14}, { -4,  14},
	{ -5,  14}, { -6,  14}, { -7,  13}, { -8,  13},
	{ -9,  12}, {-10,  11}, {-11,  11}, {-11,  10},
	{-12,  10}, {-12,   9}, {-13,   8}, {-14,   7},
	{-14,   6}, {-15,   5}, {-15,   4}, {-15,   3},
	{-15,   2}, {-15,   1}, {-15,   0}, {-15,  -1},
	{-15,  -2}, {-15,  -3}, {-15,  -4}, {-15,  -5},
	{-15,  -6}, {-14,  -7}, {-14,  -8}, {-13,  -9},
	{-12, -10}, {-12, -11}, {-11, -11}, {-11, -12},
	{-10, -12}, { -9, -13}, { -8, -14}, { -7, -14},
	{ -6, -15}, { -5, -15}, { -4, -15}, { -3, -15},
	{ -2, -15}, { -1, -15}, {  0, -15}, {  1, -15},
	{  2, -15}, {  3, -15}, {  4, -15}, {  5, -15},
	{  6, -14}, {  7, -14}, {  8, -13}, {  9, -12},
	{ 10, -12}, { 10, -11}, { 11, -11}, { 11, -10},
	{ 12,  -9}, { 13,  -8}, { 13,  -7}, { 14,  -6},
	{ 14,  -5}, { 14,  -4}, { 14,  -3}, { 14,  -2},
	{ 14,  -1}
};

namespace lve {

	FirstApp::FirstApp() {
		globalPool =
			LveDescriptorPool::Builder(lveDevice)
			.setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run() {
		std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<LveBuffer>(
				lveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		auto globalSetLayout =
			LveDescriptorSetLayout::Builder(lveDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			LveDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{
			lveDevice,
			lveRenderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout() };
		LveCamera camera{};

		auto viewerObject = LveGameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f;
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		glfwSetScrollCallback(lveWindow.getGLFWwindow(), KeyboardMovementController::updateScroll);

		while (!lveWindow.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

			currentTime = newTime;

			cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject, camera);
			cameraController.updateMouseInput(lveWindow.getGLFWwindow(), camera, frameTime);
			camera.setViewMatrix(viewerObject.transform.translation);

			float aspect = lveRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 2000.f);

			if (auto commandBuffer = lveRenderer.beginFrame()) {
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects };

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);

				// order here matters
				simpleRenderSystem.renderGameObjects(frameInfo);

				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects() {		
		const std::shared_ptr<LveModel> lveModel = std::make_unique<LveModel>(
			lveDevice,
			VoxelModelManager::loadFile("../models/grass.voxm"));
		auto testVoxelModel = LveGameObject::createGameObject();
		testVoxelModel.model = lveModel;
		testVoxelModel.transform.scale = { 1.f, 1.f, 1.f };
		testVoxelModel.transform.translation = glm::vec3(0.f, 0.f, 0.f);
		gameObjects.emplace(testVoxelModel.getId(), std::move(testVoxelModel));

	}

}  // namespace lve
