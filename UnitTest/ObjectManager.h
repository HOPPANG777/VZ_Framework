#pragma once

#include "Objects/ColorObject/ColorRect.h"
#include "Objects/ColorObject/ColorLine.h"
#include "Objects/ColorObject/ColorCircle.h"
#include "Objects/ColorObject/ColorFilledCircle.h"

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager);
public:
	void Init();

	void Update();
	void Render();

private:
	vector<shared_ptr<Object>> objects;

};

