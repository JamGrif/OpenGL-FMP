#include <fstream>
#include <sstream>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Mesh*> MeshManager::loadedModels;

Mesh::Mesh(const char* filePath)
	:m_filePath(filePath)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filePath, aiProcess_Triangulate );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "MESH->" << import.GetErrorString() << std::endl;
	}

	aiMesh* mesh = scene->mMeshes[0];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//Position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//Texcoords
		glm::vec2 vec;
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;
		vertex.TexCoords = vec;

		vertices.push_back(vertex);
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

}



std::vector<Vertex> Mesh::getVertices()
{
	return vertices;
}

std::vector<unsigned int> Mesh::getIndices()
{
	return indices;
}

const char* Mesh::getFilePath() const
{
	return m_filePath;
}


Mesh* MeshManager::loadModel(const char* filePath)
{
	//Check if model is already loaded loaded
	for (Mesh* im : loadedModels)
	{
		if (im->getFilePath() == filePath)
		{
			//std::cout << "MESHMANAGER->" << filePath << " already exists, returning loaded model" << std::endl;
			return im;
		}
	}

	//Otherwise, create new model and add it to vector
	std::cout << "MESHMANAGER->" << filePath << " is being loaded" << std::endl;

	loadedModels.push_back(new Mesh(filePath));
	return loadedModels.back();
}
