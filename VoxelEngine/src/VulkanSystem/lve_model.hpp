#pragma once

#include "lve_buffer.hpp"
#include "lve_device.hpp"

// libs
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// std
#include <memory>
#include <vector>

#include "lve_utils.hpp"

namespace lve {


struct Vertex {
    glm::vec3 position{};
    glm::vec4 color{};

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

    bool operator==(const Vertex& other) const {
        return position == other.position && color == other.color;
    }
};

class LveModel {
 public:

  struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    void insertVertex(const Vertex vertex, std::unordered_map<Vertex, uint32_t>& uniqueVertices);
  };

  LveModel(LveDevice &device, const LveModel::Builder &builder);
  ~LveModel();

  LveModel(const LveModel &) = delete;
  LveModel &operator=(const LveModel &) = delete;
  
  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

 private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);
  void createIndexBuffers(const std::vector<uint32_t> &indices);

  LveDevice &lveDevice;

  std::unique_ptr<LveBuffer> vertexBuffer;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  std::unique_ptr<LveBuffer> indexBuffer;
  uint32_t indexCount;
};
}  // namespace lve


namespace std {
    template <>
    struct hash<lve::Vertex> {
        size_t operator()(lve::Vertex const& vertex) const {
            size_t seed = 0;
            lve::hashCombine(seed, vertex.position, vertex.color);
            return seed;
        }
    };
}  // namespace std