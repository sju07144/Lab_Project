#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "mesh.h"
#include "shader.h"
#include <string>
#include "texture.h"
#include <vector>

class Model
{
public:
	Model(const char* path);
	void Draw(const Shader& shader);
	int GetFaceNum();
private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	int faceNum;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(std::vector<Texture>& outputTextures, aiMaterial* mat, aiTextureType type, std::string typeName);
};