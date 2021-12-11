#pragma once

#include "Texture.h"

class SaveableTexture
{
public:
	SaveableTexture(Ref<Texture> texture, float ratio = 1.0f, float texCoordScale = 1.0);

	virtual void Save(YAML::Emitter& emitter) const;

	static Ref<SaveableTexture> StaticLoad(YAML::Node& node);

	Ref<Texture> texture;
	float ratio;
	float texCoordScale;
};