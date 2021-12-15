#include "StaticScene.h"
#include "MeshManager.h"
#include "Renderer.h"
#include "YAMLOverloads.h"

#include <GameObjectToSphereContactGenerator.h>
#include <GameObjectToAABBContactGenerator.h>
#include <GameObjectToMeshContactGenerator.h>

#include <sstream>
#include <fstream>

StaticScene::StaticScene(Ref<Shader> shader)
	: shader(shader)
{
	this->lightMesh = MeshManager::LoadMesh("assets\\models\\ISO_Sphere.ply");
}

void StaticScene::Save(const std::string& path)
{
	if (path.empty())
	{
		return;
	}

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";

	// Save EnvMap
	if (this->envMap)
	{
		out << YAML::Key << "EnvironmentMap" << YAML::Value;
		this->envMap->Save(out);
	}

	// Save lights
	out << YAML::Key << "Lights" << YAML::Value << YAML::BeginSeq;
	for (const Ref<Light>& light : this->lights)
	{
		light->Save(out);
	}
	out << YAML::EndSeq;

	// Save meshes
	out << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq;
	std::vector<Ref<SaveableMesh>>::iterator meshIt;
	for (meshIt = this->meshes.begin(); meshIt != this->meshes.end(); meshIt++)
	{
		(*meshIt)->Save(out);
	}
	out << YAML::EndSeq;

	out << YAML::EndMap;

	std::ofstream ofs(path);
	ofs << out.c_str();
}

void StaticScene::Load(const std::string& path, CollisionContainer* collisionContainer)
{
	std::ifstream ifs(path);
	if (!ifs.good())
	{
		std::cout << "StaticScene does not exist!" << std::endl;
		return;
	}

	this->meshes.clear();
	this->lights.clear();
	this->transparentEnd = -1;

	std::stringstream ss;
	ss << ifs.rdbuf();

	shader->Bind();

	YAML::Node root = YAML::Load(ss.str());
	if (!root["Scene"])
	{
		std::cout << "Error loading scene file " << path << std::endl;
		return;
	}

	std::string sceneName = root["Scene"].as<std::string>();

	// Load Env map
	const YAML::Node& envMapNode = root["EnvironmentMap"];
	if (envMapNode)
	{
		this->envMap = EnvironmentMap::StaticLoad(envMapNode);
	}

	// Load Lights
	const YAML::Node& lightNode = root["Lights"];
	if (lightNode)
	{
		YAML::const_iterator it;
		for (it = lightNode.begin(); it != lightNode.end(); it++)
		{
			YAML::Node childNode = (*it);
			Ref<Light> light = Light::StaticLoad(childNode);

			light->SendToShader();
			this->lights.push_back(light);
		}
	}

	const YAML::Node& meshNode = root["Meshes"];
	if (meshNode)
	{
		YAML::const_iterator it;
		for (it = meshNode.begin(); it != meshNode.end(); it++)
		{
			const YAML::Node& node = (*it);
			Ref<SaveableMesh> meshData = SaveableMesh::StaticLoad(node);

			// Load collision data if the mesh has any 
			CollisionHandlerType collisionType = meshData->collider.colliderType;
			if (collisionType == CollisionHandlerType::Sphere)
			{
				collisionContainer->AddGenerator(new GameObjectToSphereContactGenerator((SphereCollider*) meshData->collider.collider, meshData->position, meshData->collider.bounciness));
			}
			else if (collisionType == CollisionHandlerType::Cuboid)
			{
				glm::mat4 transform(1.0f);
				transform *= glm::translate(glm::mat4(1.0f), meshData->position);
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				transform *= glm::scale(glm::mat4(1.0f), meshData->scale);

				collisionContainer->AddGenerator(new GameObjectToAABBContactGenerator((AABBCollider*)meshData->collider.collider, transform, meshData->collider.bounciness));
			}
			if (collisionType == CollisionHandlerType::Mesh)
			{
				glm::mat4 transform(1.0f);
				transform *= glm::translate(glm::mat4(1.0f), meshData->position);
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
				transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				transform *= glm::scale(glm::mat4(1.0f), meshData->scale);

				collisionContainer->AddGenerator(new GameObjectToMeshContactGenerator((MeshCollider*)meshData->collider.collider, transform, meshData->collider.bounciness));
			}
	
			AddMesh(meshData);
		}
	}
}

void StaticScene::AddLight(const glm::vec3& position)
{
	int lightIndex = this->lights.size();
	if (lightIndex >= MAX_LIGHTS)
	{
		std::cout << "The maximum number of lights has been reached. You must increase the MAX_LIGHTS value in LightManager.h AND the fragment shader to inccrease the number of lights." << std::endl;
		return;
	}

	Ref<Light> light = CreateRef<Light>(lightIndex);
	light->position = glm::vec4(position, 1.0f);

	shader->Bind();
	light->SendToShader();
	this->lights.push_back(light);
}

void StaticScene::AddMesh(Ref<SaveableMesh> meshData)
{
	if (!this->meshes.empty() && meshData->alphaTransparency < 1.0f) // We have to add transparent objects to the front so we can handle blend transparency
	{
		this->meshes.insert(this->meshes.begin(), meshData);
		transparentEnd++;
	}
	else
	{
		this->meshes.push_back(meshData);
	}
}

void StaticScene::OnUpdate(const glm::vec3& cameraPosition, float deltaTime)
{
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

	// Draw environment map
	if (envMap)
	{
		this->envMap->Draw(this->shader, cameraPosition, glm::vec3(10.0f, 10.0f, 10.0f));
	}

	// Draw meshes
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);

	for (int i = 1; i <= this->transparentEnd; i++) // Sort the transparent meshes (use insertion sort here since they are sorted the majority of the time)
	{
		int j = i - 1;

		Ref<SaveableMesh> cachedMeshData = this->meshes[i];
		glm::vec3 difference = cameraPosition - cachedMeshData->position;
		float currentDistance = glm::dot(difference, difference);

		glm::vec3 difference2 = cameraPosition - this->meshes[j]->position;
		float toCompare = glm::dot(difference2, difference2);

		while (j >= 0 && toCompare < currentDistance)
		{
			this->meshes[j + 1] = this->meshes[j];
			j = j - 1;
		}

		this->meshes[j + 1] = cachedMeshData;
	}

	glDisable(GL_BLEND);

	// Render opaque meshes
	int startIndex = this->transparentEnd == -1 ? 0 : this->transparentEnd + 1;
	std::vector<int> changedAlphaValues;
	for (int i = startIndex; i < this->meshes.size(); i++)
	{
		Ref<SaveableMesh> meshData = this->meshes[i];
		glm::mat4 transform(1.0f);
		transform *= glm::translate(glm::mat4(1.0f), meshData->position);
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transform *= glm::scale(glm::mat4(1.0f), meshData->scale);

		if (meshData->alphaTransparency < 1.0f || meshData->hasAlphaTransparentTexture) // Alpha transparency is now below 1, we need to flag this object to be sorted
		{
			changedAlphaValues.push_back(i);
		}

		Renderer::RenderMeshWithTextures(shader, meshData->mesh, meshData->textures, transform, meshData->alphaTransparency);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i <= this->transparentEnd; i++)
	{
		Ref<SaveableMesh> meshData = this->meshes[i];
		glm::mat4 transform(1.0f);
		transform *= glm::translate(glm::mat4(1.0f), meshData->position);
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform *= glm::rotate(glm::mat4(1.0f), meshData->orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transform *= glm::scale(glm::mat4(1.0f), meshData->scale);
		
		Renderer::RenderMeshWithTextures(shader, meshData->mesh, meshData->textures, transform, meshData->alphaTransparency);
	}

	glDisable(GL_BLEND);
}