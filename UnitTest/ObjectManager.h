#pragma once

#include "Objects/ColorRect.h"
#include "Objects/ColorLine.h"
#include "Objects/ColorCircle.h"
#include "Objects/ColorFilledCircle.h"

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

