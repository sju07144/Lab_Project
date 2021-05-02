#include "model.h"

Model::Model(const char* path)
{
	faceNum = 0;
	LoadModel(path);
}

void Model::Draw(const Shader& shader)
{
	for (auto i = 0; i != meshes.size(); ++i)
		meshes[i].Draw(shader);
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer import;
	/*
	aiProcess_Triangulate: Assimp���� ���� �ﰢ�����θ� �̷������ �ʾҴٸ� ���� ��� primitive �������� �ﰢ������ ��ȯ�϶�� ������
	aiProcess_FlipUVs: �ؽ��� ��ǥ�� y������ ��������
	aiProcess_GenNormals: ���� normal ���͵��� ������ ���� �ʴٸ� �� vertex�� ���� normal�� ������ ����
	aiProcess_SplitLargeMeshes: ū mesh���� ���� ���� ���� ���� mesh��� �����ϴ�. �������� ���� vertex ���� �ִ��� ������ ���� �� �����ϰ� ���� ���� mesh�鸸 ó�� ����
	aiProcess_OptimizeMeshes: �ݴ�� ���� mesh���� �ϳ��� ū mesh�� ��Ĩ�ϴ�. ����ȭ�� ���� Draw Call�� ���� �� �ֽ��ϴ�.
	*/

	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (auto i = 0; i != node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		faceNum += mesh->mNumFaces;
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (auto i = 0; i != node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (auto i = 0; i != mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// vertex position, normal, texture ó��
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		/* vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector; */
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector; 
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// indices ó��
	for (auto i = 0; i != mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (auto j = 0; j != face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Material mat;
	// materials ó��
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D color;
		float transparent;

		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.Ka = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.Kd = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.Ks = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_OPACITY, transparent); // mtl d value
		mat.transparent = transparent;
		
		std::vector<Texture> diffuseMaps;
		LoadMaterialTextures(diffuseMaps, material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps;
		LoadMaterialTextures(specularMaps, material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> ambientMaps;
		LoadMaterialTextures(ambientMaps, material, aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

		std::vector<Texture> normalMaps;
		LoadMaterialTextures(normalMaps, material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps;
		LoadMaterialTextures(heightMaps, material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	return Mesh(vertices, indices, textures, mat, mesh->mNumFaces);
}

void Model::LoadMaterialTextures(std::vector<Texture>& outputTextures, aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (auto i = 0; i != mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (const auto& j : textures_loaded)
		{
			if (std::strcmp(j.path.data(), str.C_Str()) == 0)
			{
				textures.push_back(j);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �ؽ�ó�� �̹� �ҷ��������� �ʴٸ� �ҷ��ɴϴ�.
			Texture texture;
			texture.id = LoadTextureUsingDirectory(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // �ҷ��� �ؽ�ó�� �����մϴ�.
		}
	}
	// outputTextures = textures;
	outputTextures = std::forward<std::vector<Texture>>(textures);
}
int Model::GetFaceNum() { return faceNum; }