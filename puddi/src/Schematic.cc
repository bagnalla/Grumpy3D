#include "Schematic.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "Material.h"
#include "Texture.h"
#include "Util.h"
#include <glm/vec4.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <iostream>

using namespace std;

namespace puddi
{
	// PUBLIC

	void Schematic::Init()
	{
		//aiLogStream stream;
		//// get a handle to the predefined STDOUT log stream and attach
		//// it to the logging system. It remains active for all further
		//// calls to aiImportFile(Ex) and aiApplyPostProcessing.
		//stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
		//aiAttachLogStream(&stream);
		//// ... same procedure, but this stream now writes the
		//// log messages to assimp_log.txt
		//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
		//aiAttachLogStream(&stream);
	}

	void Schematic::Cleanup()
	{
		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
		aiDetachAllLogStreams();
	}

	int Schematic::InitSchematic(const char *filepath, string name, std::string subdirectory)
	{
		if (schematicNodeMap.find(name) != schematicNodeMap.end())
		{
			std::cerr << "schematic with name " << name << ". already exists\n";
			return -1;
		}

		const aiScene *scene = NULL;

		if ((scene = aiImportFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
		//if ((scene = aiImportFile("model\\Millennium_Falcon.obj", aiProcessPreset_TargetRealtime_Fast)) != NULL)
		{
			SchematicNode *schematic = buildSchematic(scene, scene->mRootNode, subdirectory);

			schematicNodeMap.emplace(name, schematic);
		}
		else
		{
			std::cerr << aiGetErrorString() << std::endl;
			return -2;
		}

		// cleanup - calling 'aiReleaseImport' is important, as the library
		// keeps internal resources until the scene is freed again. Not
		// doing so can cause severe resource leaking.
		aiReleaseImport(scene);

		return 0;
	}

	SchematicNode* Schematic::GetSchematicByName(string name)
	{
		return schematicNodeMap[name];
	}

	// PRIVATE
	std::unordered_map<string, SchematicNode*> Schematic::schematicNodeMap;

	SchematicNode* Schematic::buildSchematic(const aiScene *scene, aiNode *node, std::string subdirectory)
	{
		SchematicNode *schematicNode = new SchematicNode();

		/*aiVector3D scaling;
		aiQuaterniont<float> rotation;
		aiVector3D position;

		node->mTransformation.Decompose(scaling, rotation, position);*/

		// TRANSFORM
		// WARNING: THIS CONSTRUCTOR MIGHT NOT BEHAVE THE WAY WE WANT
//		schematicNode->transform = mat4(
//			vec4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4),
//			vec4(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4),
//			vec4(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4),
//			vec4(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4)
//		);

		// may be multiple meshes per node (transform shared among them)
		for (uint i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];

			int vertexOffset = Shader::Vertices.size();

			// vertex positiosn
			std::vector<vec4> vertices;
			for (int j = 0; j < mesh->mNumVertices; ++j)
				vertices.push_back(vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f));

			// vertex normals
			std::vector<vec4> normals;
			if (mesh->HasNormals())
			{
				for (int j = 0; j < mesh->mNumVertices; ++j)
					normals.push_back(vec4(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 0.0f));
			}
			else
			{
				normals = std::vector<vec4>(vertices);
			}

			// vertex tangents and binormals
			std::vector<vec4> tangents;
			std::vector<vec4> binormals;
			if (mesh->HasTangentsAndBitangents())
			{
				for (int j = 0; j < mesh->mNumVertices; ++j)
					tangents.push_back(vec4(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z, 0.0f));
				for (int j = 0; j < mesh->mNumVertices; ++j)
					binormals.push_back(vec4(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z, 0.0f));
			}
			else
			{
				tangents = std::vector<vec4>(vertices);
				binormals = std::vector<vec4>(vertices);
			}

			// vertex texture coordinates
			std::vector<vec2> textureCoordinates;
			if (mesh->HasTextureCoords(0))
			{
				for (int j = 0; j < mesh->mNumVertices; ++j)
				{
                    //std::cout << mesh->mTextureCoords[0][j].x << " " << mesh->mTextureCoords[0][j].y << std::endl;
					textureCoordinates.push_back(vec2(mesh->mTextureCoords[0][j].x, -mesh->mTextureCoords[0][j].y));
                }
			}
			else
			{
                std::cerr << "no texture coordinates for model " << subdirectory << std::endl;
				for (int j = 0; j < mesh->mNumVertices; ++j)
					textureCoordinates.push_back(vec2(0.0f, 0.0f));
			}

			// mesh indices
			std::vector<uint> indices;
			for (int j = 0; j < mesh->mNumFaces; ++j)
			{
				auto face = mesh->mFaces[j];
				if (face.mNumIndices == 3)
				{
					indices.push_back(face.mIndices[0] + vertexOffset);
					indices.push_back(face.mIndices[1] + vertexOffset);
					indices.push_back(face.mIndices[2] + vertexOffset);
				}
			}

			int indexOffset = Shader::VertexIndices.size();
			int indexCount = indices.size();

			// ADD ALL VERTEX DATA TO GLOBAL ARRAYS
			Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());
			Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
			Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
			Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
			Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoordinates.begin(), textureCoordinates.end());
			Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

			// MATERIAL
			auto aiMat = scene->mMaterials[mesh->mMaterialIndex];
			Material mat;

			std::vector<aiMaterialProperty*> matProps;
			for (int j = 0; j < aiMat->mNumProperties; ++j)
			{
				matProps.push_back(aiMat->mProperties[j]);
			}

			aiColor3D color(0.f, 0.f, 0.f);
			aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
			mat.ambient = vec4(color.r, color.g, color.b, 1.0f);
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mat.diffuse = vec4(color.r, color.g, color.b, 1.0f);
			aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
			mat.specular = vec4(color.r, color.g, color.b, 1.0f);

			float shininess;
			aiMat->Get(AI_MATKEY_SHININESS, shininess);
			mat.shininess = shininess;


			// TEXTURE
			aiString texturePath;
			aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			aiString bumpMapPath;
			aiMat->GetTexture(aiTextureType_HEIGHT, 0, &bumpMapPath);

			// CREATE AND ADD MESH OBJECT TO SCHEMATIC
			//VertexMesh vMesh(NULL, Material::Rubber(vec4(1.0f, 1.0f, 1.0f, 1.0f)), indexOffset, indexCount, false);
			VertexMesh vMesh(NULL, Material::None(), indexOffset, indexCount, false);
			std::string texName = texturePath.C_Str();
			if (texName != "")
			{
				std::string texPath = "textures/" + (subdirectory != "" ? subdirectory + "/" : "") + texName;
				std::string bumpPath = "bumpmaps/" + (subdirectory != "" ? subdirectory + "/" : "") + std::string(bumpMapPath.C_Str());
				GLuint texture = 0;
				if (texName != "")
					texture = Texture::LoadTexture(texName.c_str(), texPath.c_str(), (bumpMapPath.length != 0 ? bumpPath.c_str() : nullptr));
				else
					vMesh.SetMaterial(mat);
				vMesh.SetTexture(texture);
			}

			schematicNode->vertexMeshes.push_back(vMesh);
		}

		// recursively build children schematic nodes
		for (uint i = 0; i < node->mNumChildren; ++i)
		{
			SchematicNode *childNode = buildSchematic(scene, node->mChildren[i], subdirectory);
			if (childNode != nullptr)
				schematicNode->children.push_back(childNode);
		}

		return schematicNode;
	}
}
