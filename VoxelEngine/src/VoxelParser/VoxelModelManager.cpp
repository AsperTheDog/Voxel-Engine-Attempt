#include "VoxelModelManager.h"

#include <fstream>
#include <iostream>


lve::LveModel::Builder VoxelModelManager::loadFile(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::out | std::ios::binary);
	if (!file) {
		throw std::runtime_error("Could not load voxel model '" + filepath + "'");
	}

	uint32_t vertexCount, indexCount;
	file.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));
	file.read(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));

	uint16_t sx, sy, sz;
	file.read(reinterpret_cast<char*>(&sx), sizeof(sx));
	file.read(reinterpret_cast<char*>(&sy), sizeof(sy));
	file.read(reinterpret_cast<char*>(&sz), sizeof(sz));
	glm::uvec3 shape(sx, sy, sz);

	std::vector<Voxel> voxels;
	voxels.reserve(static_cast<size_t>(sx) * sy * sz);

	for (size_t x = 0; x < shape.x; x++)
	{
		for (size_t y = 0; y < shape.y; y++)
		{
			for (size_t z = 0; z < shape.z; z++)
			{
				Voxel vox{};
				uint8_t alpha;
				file.read(reinterpret_cast<char*>(&alpha), sizeof(alpha));
				vox.data.a = alpha;
				vox.pos.x = x;
				vox.pos.y = y;
				vox.pos.z = z;
				if (vox.data.a != 0)
				{
					uint8_t rgb[3] = { 0, 0, 0 };
					file.read(reinterpret_cast<char*>(&rgb), sizeof(rgb));
					vox.data.r = rgb[0];
					vox.data.g = rgb[1];
					vox.data.b = rgb[2];
					file.read(reinterpret_cast<char*>(&vox.phantoms), sizeof(vox.phantoms));
				}
				voxels.push_back(vox);
			}
		}
	}

	file.close();
	if (!file.good()) {
		throw std::runtime_error("Error while reading voxel model '" + filepath + "'");
	}

	std::cout << "Loading model " << filepath << " topography using " << voxels.size() << " voxels\n";
	lve::LveModel::Builder builder = VoxelModelManager::getBuilderFromVoxels(voxels, shape, vertexCount, indexCount);
	std::cout << "Loaded model " << filepath << " with " << builder.vertices.size() << " vertices and " << builder.indices.size() << " indices" << "\n";
	return builder;
}

bool VoxelModelManager::saveFile(lve::LveModel& model, const std::string& filepath)
{
	return false;
}

void loadQuad(
	const glm::ivec3 coord, 
	const glm::vec4 color, 
	lve::LveModel::Builder& builder, 
	std::unordered_map<lve::Vertex, uint32_t>& uniqueVertices,
	Faces face)
{{
	lve::Vertex vertex;
	
	vertex.color = color;

	//First triangle
	vertex.position = coord + XYZRel[offIndices[face][0]];
	builder.insertVertex(vertex, uniqueVertices);
	vertex.position = coord + XYZRel[offIndices[face][1]];
	builder.insertVertex(vertex, uniqueVertices);
	vertex.position = coord + XYZRel[offIndices[face][2]];
	builder.insertVertex(vertex, uniqueVertices);

	//Second triangle
	vertex.position = coord + XYZRel[offIndices[face][3]];
	builder.insertVertex(vertex, uniqueVertices);
	vertex.position = coord + XYZRel[offIndices[face][4]];
	builder.insertVertex(vertex, uniqueVertices);
	vertex.position = coord + XYZRel[offIndices[face][5]];
	builder.insertVertex(vertex, uniqueVertices);
	}
}

bool opaqueNeighbor(const glm::ivec3& coords, const glm::uvec3& shape, const std::vector<Voxel>& voxels)
{
	if (coords.x < 0 || coords.y < 0 || coords.z < 0) return false;
	if (coords.x >= shape.x || coords.y >= shape.y || coords.z >= shape.z) return false;
	
	const size_t idx = 
		 static_cast<size_t>(coords.z) + 
		(static_cast<size_t>(coords.y) * static_cast<size_t>(shape.z)) +
		(static_cast<size_t>(coords.x) * static_cast<size_t>(shape.z) * static_cast<size_t>(shape.y));
	return idx < voxels.size() && voxels[idx].data.a == 255;
}

lve::LveModel::Builder VoxelModelManager::getBuilderFromVoxels(
	std::vector<Voxel>& voxels, 
	const glm::uvec3& shape,
	const uint32_t vCount,
	const uint32_t iCount
)
{
	lve::LveModel::Builder builder{};
	builder.indices.reserve(iCount);
	builder.vertices.reserve(vCount);
	std::unordered_map<lve::Vertex, uint32_t> uniqueVertices{};
	uniqueVertices.reserve(vCount);
	for (const auto& [coord, data, phantoms]: voxels)
	{
		if (data.a == 0) continue;
		const auto color = glm::vec4(data) / 255.f;
		for (int f = Faces::fst; f < Faces::lst + 1; f++)
		{
			const auto face = static_cast<Faces>(f);
			if (!opaqueNeighbor(coord + neighOffsets[f], shape, voxels) && (phantoms & 1 << face) == 0)
			{
				loadQuad(coord, color, builder, uniqueVertices, face);
			}
		}
	}
	return builder;
}

