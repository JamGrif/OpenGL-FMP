#include <fstream>
#include <sstream>
#include <glm\glm.hpp>

#include "ModelLoader.h"

std::vector<ImportedModel*> MeshManager::loadedModels;

ImportedModel::ImportedModel(const char* filePath)
{
	m_filePath = filePath;
	ModelLoader ml = ModelLoader();
	ml.parseOBJ(filePath);

	numVertices = ml.getNumVertices();

	std::vector<float> verts = ml.getVertices();
	std::vector<float> tcs = ml.getTextureCoordinates();
	std::vector<float> normals = ml.getNormals();

	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}

int ImportedModel::getNumVertices() const
{
	return numVertices;
}

std::vector<glm::vec3> ImportedModel::getVertices() const
{
	return vertices;
}

std::vector<glm::vec2> ImportedModel::getTextureCoords() const
{
	return texCoords;
}

std::vector<glm::vec3> ImportedModel::getNormals() const
{
	return normalVecs;
}

ModelLoader::ModelLoader()
{
}


void ModelLoader::parseOBJ(const char* filePath)
{
	float x, y, z;
	std::string content;

	std::ifstream fileStream(filePath, std::ios::in);
	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0)							// vertex position ("v")
		{
			std::stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z;								// extract the vertex position values
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}

		if (line.compare(0, 2, "vt") == 0)							// texture coordinates ("vt)
		{
			std::stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y;										// extract texture coordinate values
			stVals.push_back(x);
			stVals.push_back(y);
		}

		if (line.compare(0, 2, "vn") == 0)							// vertex normals ("vn")
		{
			std::stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;								// extract the normal vector values
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}

		if (line.compare(0, 2, "f ") == 0)							// triangle faces ("f")
		{
			std::string oneCorner, v, t, n;
			std::stringstream ss(line.erase(0, 2));

			for (int i = 0; i < 3; i++)
			{
				std::getline(ss, oneCorner, ' ');						// extract triangle face references
				std::stringstream oneCornerSS(oneCorner);
				std::getline(oneCornerSS, v, '/');
				std::getline(oneCornerSS, t, '/');
				std::getline(oneCornerSS, n, '/');

				int vertRef = (stoi(v) - 1) * 3;					// stoi converts string to int
				int tcRef = (stoi(t) - 1) * 2;
				int normRef = (stoi(n) - 1) * 3;

				triangleVerts.push_back(vertVals[vertRef]);			// build vector of vertices
				triangleVerts.push_back(vertVals[vertRef + 1]);
				triangleVerts.push_back(vertVals[vertRef + 2]);

				textureCoords.push_back(stVals[tcRef]);				// build vector of texture coords
				textureCoords.push_back(stVals[tcRef+1]);

				normals.push_back(normVals[normRef]);				// build vector of normals
				normals.push_back(normVals[normRef + 1]);
				normals.push_back(normVals[normRef + 2]);
			}
		}
	}
}

int ModelLoader::getNumVertices() const
{
	return (triangleVerts.size() / 3);
}

std::vector<float> ModelLoader::getVertices() const
{
	return triangleVerts;
}

std::vector<float> ModelLoader::getTextureCoordinates() const
{
	return textureCoords;
}

std::vector<float> ModelLoader::getNormals() const
{
	return normals;
}

ImportedModel* MeshManager::loadModel(const char* filePath)
{
	//Check if model is already loaded loaded
	for (ImportedModel* im : loadedModels)
	{
		if (im->m_filePath == filePath)
		{
			//std::cout << "MESHMANAGER->" << filePath << " already exists, returning loaded model" << std::endl;
			return im;
		}
	}

	//Otherwise, create new model and add it to vector
	std::cout << "MESHMANAGER->" << filePath << " is being loaded" << std::endl;

	loadedModels.push_back(new ImportedModel(filePath));
	return loadedModels.back();
}
