#pragma once
#include <string>
#include <unordered_map>

#include "VulkanSystem/lve_model.hpp"

inline glm::ivec3 XYZRel[8] = {
	glm::ivec3(0, 0, 0),
	glm::ivec3(1, 0, 0),
	glm::ivec3(0, 1, 0),
	glm::ivec3(1, 1, 0),
	glm::ivec3(0, 0, 1),
	glm::ivec3(1, 0, 1),
	glm::ivec3(0, 1, 1),
	glm::ivec3(1, 1, 1),
};

inline uint8_t offIndices[6][6] = {
	{4, 5, 0, 5, 0 ,1},
	{2, 3, 6, 3, 6, 7},
	{0, 1, 2, 1, 2, 3},
	{5, 4, 7, 4, 7, 6},
	{4, 0, 6, 0, 6, 2},
	{1, 5, 3, 5, 3, 7}
};

inline glm::ivec3 neighOffsets[] = {
	glm::ivec3(0, -1, 0),
	glm::ivec3(0, 1, 0),
	glm::ivec3(0, 0, -1),
	glm::ivec3(0, 0, 1),
	glm::ivec3(-1, 0, 0),
	glm::ivec3(1, 0, 0)
};

enum Faces
{
	UP = 0,
	DOWN = 1,
	NORTH = 2,
	SOUTH = 3,
	WEST = 4,
	EAST = 5,

	fst = UP,
	lst = EAST
};

struct Voxel
{
	glm::ivec3 pos;
	glm::uvec4 data;
	uint8_t phantoms;
};

class VoxelModelManager
{
public:
	static lve::LveModel::Builder loadFile(const std::string& filepath);
	static bool saveFile(lve::LveModel& model, const std::string& filepath);
private:
	static lve::LveModel::Builder getBuilderFromVoxels(std::vector<Voxel>& voxels, const glm::uvec3& shape, uint32_t vCount, uint32_t iCount);
};

