#include "stdafx.h"
#include "LumberjackPlayer.h"

LumberjackPlayer::LumberjackPlayer(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
    : AnimationRect(position, scale, rotation, path)
{
    // 플레이어는 조작 가능하도록 MoveComponent를 추가합니다.
    AddComponent(make_shared<MoveComponent>(true));
}

LumberjackPlayer::~LumberjackPlayer()
{
}

void LumberjackPlayer::Update()
{
	BASE::Update();
}

void LumberjackPlayer::Render()
{
	BASE::Render();
}
