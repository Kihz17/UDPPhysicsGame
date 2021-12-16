#include "SaveableMesh.h"
#include "MeshManager.h"
#include "YAMLOverloads.h"

SaveableMesh::SaveableMesh(Ref<Mesh> mesh)
	: mesh(mesh),
	position(0.0f),
	orientation(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	alphaTransparency(1.0f),
	hasAlphaTransparentTexture(false),
	collider(ColliderType::None, nullptr, 0.0f)
{

}

SaveableMesh::~SaveableMesh()
{

}

void SaveableMesh::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "UUID" << YAML::Value << this->uuid;
	emitter << YAML::Key << "Path" << YAML::Value << this->mesh->GetPath();
	emitter << YAML::Key << "Position" << YAML::Value << this->position;
	emitter << YAML::Key << "Orientation" << YAML::Value << this->orientation;
	emitter << YAML::Key << "Scale" << YAML::Value << this->scale;
	emitter << YAML::Key << "AlphaTransparency" << YAML::Value << this->alphaTransparency;

	emitter << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
	for (const Ref<SaveableTexture>& textureData : this->textures)
	{
		textureData->Save(emitter);
	}
	emitter << YAML::EndSeq;

	if (collider.colliderType != ColliderType::None) 
	{
		emitter << YAML::Key << "Collider" << YAML::Value;
		collider.Save(emitter);
	}

	emitter << YAML::EndMap;
}

Ref<SaveableMesh> SaveableMesh::StaticLoad(const YAML::Node& node)
{
	std::string path = node["Path"].as<std::string>();
	Ref<Mesh> mesh = MeshManager::LoadMesh(path);

	Ref<SaveableMesh> meshData = CreateRef<SaveableMesh>(mesh);

	// Load textures
	const YAML::Node& textureNode = node["Textures"];
	if (textureNode)
	{
		YAML::const_iterator it;
		for (it = textureNode.begin(); it != textureNode.end(); it++)
		{
			YAML::Node childNode = (*it);
			Ref<SaveableTexture> textureData = SaveableTexture::StaticLoad(childNode);
			meshData->AddTexture(textureData);
		}
	}

	meshData->uuid = node["UUID"].as<uint64_t>();
	meshData->position = node["Position"].as<glm::vec3>();
	meshData->orientation = node["Orientation"].as<glm::vec3>();
	meshData->scale = node["Scale"].as<glm::vec3>();
	meshData->alphaTransparency = node["AlphaTransparency"].as<float>();

	const YAML::Node& colliderNode = node["Collider"];
	if (colliderNode)
	{
		meshData->collider = SaveableCollider::StaticLoad(mesh, colliderNode);
	}

	return meshData;
}

void SaveableMesh::AddTexture(Ref<Texture> texture, float ratio, float scale)
{
	Ref<SaveableTexture> textureData = CreateRef<SaveableTexture>(texture, ratio, scale);

	if (texture->GetType() == TextureType::Heightmap)
	{
		if (!this->textures.empty() && this->textures[0]->texture->GetType() == TextureType::Heightmap)
		{
			std::cout << "WARNING: Tried to add multiple heightmap textures to one mesh!" << std::endl;
			return;
		}

		this->textures.insert(this->textures.begin(), textureData); // Make sure heightmap is at the front
		return;
	}
	else if (texture->GetType() == TextureType::Alpha)
	{
		this->hasAlphaTransparentTexture = true;
	}

	this->textures.push_back(textureData);
}

void SaveableMesh::AddTexture(Ref<SaveableTexture> textureData)
{
	if (textureData->texture->GetType() == TextureType::Heightmap)
	{
		if (!this->textures.empty() && this->textures[0]->texture->GetType() == TextureType::Heightmap)
		{
			std::cout << "WARNING: Tried to add multiple heightmap textures to one mesh!" << std::endl;
			return;
		}

		this->textures.insert(this->textures.begin(), textureData); // Make sure heightmap is at the front
		return;
	}
	else if (textureData->texture->GetType() == TextureType::Alpha)
	{
		this->hasAlphaTransparentTexture = true;
	}

	this->textures.push_back(textureData);
}