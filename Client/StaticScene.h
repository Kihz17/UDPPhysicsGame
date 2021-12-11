#pragma once

#include "SaveableMesh.h"
#include "UUID.h"
#include "EnvironmentMap.h"
#include "Light.h"
#include "Camera.h"
#include "CollisionContainer.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <algorithm>

class StaticScene
{
public:
	StaticScene(Ref<Shader> shader);

	void Save(const std::string& path);
	void Load(const std::string& path, CollisionContainer* collisionContainer);

	inline void SetEnvMap(const Ref<EnvironmentMap> envMap) { this->envMap = envMap; }

	void AddLight(const glm::vec3& position);

	void AddMesh(Ref<SaveableMesh> meshData);

	void OnUpdate(const glm::vec3& cameraPosition, float deltaTime);
private:
	std::vector<Ref<SaveableMesh>> meshes;
	int transparentEnd;

	Ref<EnvironmentMap> envMap;
	std::vector<Ref<Light>> lights;

	Ref<Shader> shader;
	Ref<Mesh> lightMesh;
	static const unsigned int MAX_LIGHTS = 10; // This must match the value in the fragment shader
};