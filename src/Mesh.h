#pragma once

#include <vector>
#include <iostream>
#include "glm/glm.hpp"


class Mesh
{
public:
	Mesh(const char* filePath);

	int getNumVertices() const;

	std::vector<glm::vec3> getVertices() const;
	std::vector<glm::vec2> getTextureCoords() const;
	std::vector<glm::vec3> getNormals() const;

	const char* getFilePath() const;

private:

	const char* m_filePath;

	int numVertices;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;

};


class MeshLoader
{
public:
	MeshLoader();
	
	void parseOBJ(const char* filePath);

	int getNumVertices() const;

	std::vector<float> getVertices() const;
	std::vector<float> getTextureCoordinates() const;
	std::vector<float> getNormals() const;

private:

	//Values read in from .OBJ file
	std::vector<float> vertVals;
	std::vector<float> stVals;
	std::vector<float> normVals;

	//Values stored for later use as vertex attributes
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;

};

class MeshManager
{
public:

	static Mesh* loadModel(const char* filePath);

private:

	static std::vector<Mesh*> loadedModels;

	MeshManager();
};

