#pragma once

#include "SceneMeshData.h"
#include "Light.h"

class Scene;
class ScenePanel
{
public:
	ScenePanel(Scene* scene);
	virtual ~ScenePanel();

	inline void SetMeshData(Ref<SceneMeshData> meshData) { this->meshData = meshData; }
	inline void SetLight(Ref<Light> light) { this->light = light; }
	void OnUpdate(float deltaTime);

	inline Ref<SceneMeshData> GetEditMesh() const { return this->meshData; }
private:
	Scene* scene;
	Ref<SceneMeshData> meshData;
	Ref<Light> light;
	std::string sceneName;
	std::string sceneLoadName;
	std::string textureName;
	std::string lightType;

	int selectedTexture;
	int selectedFilter;
	int selectedWrap;
	bool genMipMaps;

	static const char* textureTypes[];
	static const char* filterTypes[];
	static const char* wrapTypes[];
};