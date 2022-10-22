#pragma once
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_set>

#include "VoxelModelManager.h"

inline uint8_t advancePercentage(const size_t count, const size_t total, const uint8_t last)
{
	const double portion = static_cast<double>(count) / static_cast<double>(total);
	const uint8_t perc = std::round(portion * 100);
	if (perc > last)
	{
		std::cout << static_cast<uint16_t>(perc) << "%" << "\n";
	}
	return perc;
}

inline void insertVertex(
	const lve::Vertex& vtx, 
	uint32_t& indices, 
	std::unordered_set<lve::Vertex>& vertices)
{
	if (!vertices.contains(vtx))
	{
		vertices.insert(vtx);
	}
	indices++;
}

inline bool opaqueNeighbor(
	const Faces face,
	const glm::ivec3 coords,
	const glm::uvec3 shape,
	const std::vector<Voxel>& voxels)
{
	if (coords.x < 0 || coords.y < 0 || coords.z < 0) return false;
	if (coords.x >= shape.x || coords.y >= shape.y || coords.z >= shape.z) return false;

	const glm::vec3 off = coords + neighOffsets[face];
	const size_t idx = off.z + (off.y * shape.z) + (off.x * shape.z * shape.y);
	return idx < voxels.size() && voxels[idx].data.a == 255;
}

inline uint32_t processVertices(
	std::unordered_set<lve::Vertex>& vertices,
	const std::vector<Voxel>& voxels,
	const glm::uvec3 shape
)
{
	uint32_t indices = 0;

	size_t count = 0;
	uint8_t lastPerc = 0;
	for (const auto& [pos, data, phantoms] : voxels)
	{
		lastPerc = advancePercentage(count, voxels.size(), lastPerc);
		count++;
		if (data.w == 0) continue;

		for (int f = Faces::fst; f < Faces::lst + 1; f++)
		{
			if (const auto face = static_cast<Faces>(f); 
				!opaqueNeighbor(face, pos, shape, voxels) && (phantoms & (1 << f)) == 0)
			{
				insertVertex({ pos + XYZRel[offIndices[face][0]], data }, indices, vertices);
				insertVertex({ pos + XYZRel[offIndices[face][1]], data }, indices, vertices);
				insertVertex({ pos + XYZRel[offIndices[face][2]], data }, indices, vertices);

				insertVertex({ pos + XYZRel[offIndices[face][3]], data }, indices, vertices);
				insertVertex({ pos + XYZRel[offIndices[face][4]], data }, indices, vertices);
				insertVertex({ pos + XYZRel[offIndices[face][5]], data }, indices, vertices);
			}
		}
	}
	return indices;
}

inline void generateSample(
	const std::string& filename, 
	bool (*blockCheck)(const glm::ivec3&, const glm::uvec3&),
	uint8_t (*genPhantom)(const glm::ivec3&, const glm::uvec3&),
	const glm::uvec3 shape)
{
	std::unordered_set<lve::Vertex> vertices;
	std::vector<Voxel> voxels;

	size_t total = static_cast<size_t>(shape.x) * shape.y * shape.z;
	voxels.reserve(total);

	float xRate = 255.0f / shape.x;
	float yRate = 255.0f / shape.y;
	float zRate = 255.0f / shape.z;
	size_t count = 0;
	uint8_t lastPerc = 0;
	uint32_t voxelCounts[2] = { 0, 0 };
	for (int x = 0; x < shape.x; x++)
	{
		for (int y = 0; y < shape.y; y++)
		{
			for (int z = 0; z < shape.z; z++)
			{
				Voxel vox{};
				vox.pos.x = x;
				vox.pos.y = y;
				vox.pos.z = z;
				if (blockCheck(vox.pos, shape))
				{
					vox.data.r = static_cast<uint64_t>(std::floor(x * xRate));
					vox.data.g = static_cast<uint64_t>(std::floor(y * yRate));
					vox.data.b = static_cast<uint64_t>(std::floor(z * zRate));
					vox.data.a = 255;
					vox.phantoms = genPhantom(vox.pos, shape);
					voxels.push_back(vox);
					voxelCounts[0]++;
				}
				else
				{
					voxelCounts[1]++;
					vox.data.a = 0;
					voxels.push_back(vox);
				}
				lastPerc = advancePercentage(count, total, lastPerc);
				count++;
			}
		}
	}
	std::cout << "Generated voxel map -> " << voxelCounts[0] << " solid, " << voxelCounts[1] << " air. Total: " << voxels.size() << "\n";
	uint32_t indices = processVertices(vertices, voxels, shape);
	uint32_t verticeCount = vertices.size();
	std::cout << "Vertices: " << verticeCount << "\n" << "Indices: " << indices << "\n";
	vertices.clear();
	std::ofstream wf(filename, std::ios::out | std::ios::binary);
	if (!wf) {
		std::cout << "Cannot open file!" << "\n";
		return;
	}

	wf.write(reinterpret_cast<char*>(&verticeCount), sizeof(verticeCount));
	wf.write(reinterpret_cast<char*>(&indices), sizeof(indices));

	uint16_t xSize = static_cast<uint16_t>(shape.x);
	wf.write(reinterpret_cast<char*>(&xSize), sizeof(xSize));
	uint16_t ySize = static_cast<uint16_t>(shape.y);
	wf.write(reinterpret_cast<char*>(&ySize), sizeof(ySize));
	uint16_t zSize = static_cast<uint16_t>(shape.z);
	wf.write(reinterpret_cast<char*>(&zSize), sizeof(zSize));

	count = 0;
	lastPerc = 0;
	for (const auto& [pos, data, phant] : voxels)
	{
		uint8_t alpha = static_cast<uint8_t>(data.a);
		wf.write(reinterpret_cast<char*>(&alpha), sizeof(alpha));
		if (data.a != 0)
		{
			uint8_t red = static_cast<uint8_t>(data.r);
			wf.write(reinterpret_cast<char*>(&red), sizeof(red));
			uint8_t green = static_cast<uint8_t>(data.g);
			wf.write(reinterpret_cast<char*>(&green), sizeof(green));
			uint8_t blue = static_cast<uint8_t>(data.b);
			wf.write(reinterpret_cast<char*>(&blue), sizeof(blue));
			uint8_t phantoms = phant;
			wf.write(reinterpret_cast<char*>(&phantoms), sizeof(phantoms));
		}
		lastPerc = advancePercentage(count, voxels.size(), lastPerc);
		count++;
	}
	wf.close();
	std::cout << "Task completed for file " << filename << "\n";
}


