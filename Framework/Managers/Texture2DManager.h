#pragma once
#include "../Framework.h"

class Texture2DManager
{
	DECLARE_SINGLETON(Texture2DManager);

public:
	void Load(Texture2D* texture);
	void Delete();

private:
	static vector<TextureDesc> descs;

};

