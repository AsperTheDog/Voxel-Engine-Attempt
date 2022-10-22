#pragma once

#include "VulkanSystem/lve_descriptors.hpp"
#include "VulkanSystem/lve_device.hpp"
#include "VulkanSystem/lve_game_object.hpp"
#include "VulkanSystem/lve_renderer.hpp"
#include "VulkanSystem/lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class FirstApp {
 public:
  static constexpr int WIDTH = 1920;
  static constexpr int HEIGHT = 1080;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

 private:
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "Voxel Engine"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  // note: order of declarations matters
  std::unique_ptr<LveDescriptorPool> globalPool{};
  LveGameObject::Map gameObjects;
};
}  // namespace lve
