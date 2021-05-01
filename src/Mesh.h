#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadMesh(const char* filePath);

	std::vector<Vertex> getVertices() const;
	std::vector<unsigned int> getIndices() const;
	const char* getFilePath() const;

private:

	const char*					m_filePath;

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;

};

class MeshManager
{
public:

	static Mesh* loadModel(const char* filePath);

	static void clearMeshes();

private:

	static std::vector<Mesh*> loadedModels;

	MeshManager();
};

