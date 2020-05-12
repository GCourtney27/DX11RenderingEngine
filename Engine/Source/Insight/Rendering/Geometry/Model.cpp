#include "ie_pch.h"

#include "Model.h"
#include "Insight/Utilities/String_Helper.h"

namespace Insight {


	Model::Model(const std::string& path)
	{
		LoadModelFromFile(path);
	}

	Model::~Model()
	{
		//Destroy();
	}

	bool Model::Init(const std::string& path)
	{
		m_Directory = StringHelper::GetDirectoryFromPath(path);
		m_FileName = StringHelper::GetFileExtension(path);

		return LoadModelFromFile(path);
	}

	void Model::Draw()
	{
		for (UINT i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Draw();
	}

	void Model::Destroy()
	{
		for (UINT i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Destroy();
	}

	bool Model::LoadModelFromFile(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(
			path, 
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded
		);

		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
		{
			IE_CORE_TRACE("Assimp import error: {0}", importer.GetErrorString());
			return false;
		}

		ProcessNode_r(pScene->mRootNode, pScene);
		return true;
	}

	void Model::ProcessNode_r(aiNode* pNode, const aiScene* pScene)
	{
		for (UINT i = 0; i < pNode->mNumMeshes; i++)
		{
			aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(pMesh, pScene)); // BUG: Copies mesh into vector, ownership mismath crashes program when destructor is called
		}
		for (UINT i = 0; i < pNode->mNumChildren; i++)
		{
			ProcessNode_r(pNode->mChildren[i], pScene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
	{
		using namespace DirectX;
		std::vector<Vertex> verticies;
		std::vector<DWORD> indices;
		//std::vector<Texture> textures;

		// Load Verticies
		for (UINT i = 0; i < pMesh->mNumVertices; i++)
		{
			Vertex vertex;
			// Position
			vertex.Position.x = pMesh->mVertices[i].x;
			vertex.Position.y = pMesh->mVertices[i].y;
			vertex.Position.z = pMesh->mVertices[i].z;

			// Texture Coords/Tangents
			if (pMesh->mTextureCoords[0])
			{
				vertex.TexCoords.x = (float)pMesh->mTextureCoords[0][i].x;
				vertex.TexCoords.y = (float)pMesh->mTextureCoords[0][i].y;

				vertex.Tangent.x = pMesh->mTangents[i].x;
				vertex.Tangent.y = pMesh->mTangents[i].y;
				vertex.Tangent.z = pMesh->mTangents[i].z;
				
				vertex.BiTangent.x = pMesh->mBitangents[i].x;
				vertex.BiTangent.y = pMesh->mBitangents[i].y;
				vertex.BiTangent.z = pMesh->mBitangents[i].z;
			}
			else
			{
				vertex.TexCoords = XMFLOAT2(0.0f, 0.0f);
				vertex.Tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
				vertex.BiTangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
			// Normals
			vertex.Normal.x = (float)pMesh->mNormals[i].x;
			vertex.Normal.y = (float)pMesh->mNormals[i].y;
			vertex.Normal.z = (float)pMesh->mNormals[i].z;

			verticies.push_back(vertex);
		}

		// Load Indices
		for (UINT i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace face = pMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Load Textures
		/*if (pMesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}*/
		return Mesh(verticies, indices);

	}

}
