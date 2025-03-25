#include "stdafx.h"
#include "LumberAxe.h"

LumberAxe::LumberAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: TextureRect(position, scale, rotation, path)
{
	lumberAxeComponent = make_shared<LumberAxeComponent>();
	AddComponent(lumberAxeComponent);
	AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), scale));
}

LumberAxe::~LumberAxe()
{

}

void LumberAxe::Update()
{
	BASE::Update();

    // 'E' 키 입력과 타겟이 설정되어 있을 경우 회수 시작
    if (INPUT->KeyPressed('E'))
    {
		lumberAxeComponent->Recall();
    }
}

void LumberAxe::Render()
{
	BASE::Render();
}