#pragma once
#include "Structs/TextureDesc.h"
#include "Renders/RenderResources/Texture2D.h"
#include "Utilities/Path.h"

class Texture2D;

class Texture2DManager
{
	DECLARE_SINGLETON(Texture2DManager);

public:
	void Load(Texture2D* texture);
	void Delete();

private:
	static std::vector<TextureDesc> descs;

};

