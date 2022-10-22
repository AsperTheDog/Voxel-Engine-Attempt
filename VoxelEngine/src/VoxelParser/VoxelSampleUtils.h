#pragma once
#include <glm/fwd.hpp>

#include "VoxelModelManager.h"


inline glm::uint8_t phantomHoriz(const glm::ivec3& coord, const glm::uvec3& size)
{
	uint8_t phantoms = 0;
	phantoms += coord.z == 0 ? 1 << Faces::NORTH : 0;
	phantoms += coord.z == size.z - 1 ? 1 << Faces::SOUTH : 0;
	phantoms += coord.x == 0 ? 1 << Faces::WEST : 0;
	phantoms += coord.x == size.x - 1 ? 1 << Faces::EAST : 0;
	return phantoms;
}

inline uint8_t phantomAlways(const glm::ivec3& coord, const glm::uvec3& size)
{
	return 0;
}

inline bool tru(const glm::ivec3& coord, const glm::uvec3& size)
{
	return true;
}

inline bool circle(const glm::ivec3& coord, const glm::uvec3& size)
{
	return std::pow((coord.x - (size.x / 2.0f)), 2) + std::pow((coord.y - (size.y / 2.0f)), 2) + std::pow((coord.z - (size.z / 2.0f)), 2) < std::pow((size.x / 2.1f), 2);
}

inline bool ripple(const glm::ivec3& coord, const glm::uvec3& size)
{
	return coord.y > (std::sin(10 * (std::pow(((coord.x - (size.x / 2.0f)) / 200.0f), 2) + std::pow(((coord.z - (size.y / 2.0f)) / 200.0f), 2))) * ((1 / ((coord.z + (size.z / 2.0f)) + (coord.x + (size.x / 2.0f)))) * 20000.0f)) + (size.y / 2.0f);
}

inline bool absSines(const glm::ivec3& coord, const glm::uvec3& size)
{
	return coord.y > (std::abs(std::sin(coord.x / 30.0f)) + std::abs(std::sin(coord.z / 30.0f))) * (size.y / 4) + (size.y / 2);
}

inline bool sines(const glm::ivec3& coord, const glm::uvec3& size)
{
	return coord.y > (std::sin(coord.x / 10.0f) + std::sin(coord.z / 10.0f)) * (size.y / 4) + (size.y / 2);
}

inline bool randomVoxels(const glm::ivec3& coord, const glm::uvec3& size)
{
	constexpr auto RANDVAL = 0.06f;

	return rand() < (RAND_MAX * RANDVAL);
}
