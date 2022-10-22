#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace lve {

struct GlobalUbo {
  glm::mat4 projection{1.f};
  glm::mat4 view{1.f};
};

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  LveGameObject::Map &gameObjects;
};
}  // namespace lve
