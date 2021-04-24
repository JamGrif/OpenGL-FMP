#pragma once

#include <vector>
#include <iostream>
#include "glm/glm.hpp"



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh
{
public:
	Mesh(const char* filePath);

	

	std::vector<Vertex> getVertices();
	std::vector<unsigned int> getIndices();
	const char* getFilePath() const;

private:

	const char* m_filePath;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	

};




class MeshManager
{
public:

	static Mesh* loadModel(const char* filePath);

private:

	static std::vector<Mesh*> loadedModels;

	MeshManager();
};

