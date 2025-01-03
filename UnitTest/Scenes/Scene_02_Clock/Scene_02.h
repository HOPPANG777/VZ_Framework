#pragma once

#include "Interfaces/IScene.h"
#include "Objects/ColorObject/ColorLine.h"
#include "Objects/ColorObject/ColorCircle.h"

class Scene_02 : public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Awake() override;
	void Start() override;

	void Destroy() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void Render() override;
	void PostRender() override;

	void ImGUI() override;

private:
	float CalculateHourAngle(const int hour, const int minute) const
	{
		return (hour % 12 + minute / 60.0f) * 30.0f; // 360도 / 12시간 = 30도
	}

	float CalculateMinuteAngle(const int minute, const int second) const
	{
		return (minute + second / 60.0f) * 6.0f; // 360도 / 60분 = 6도
	}

	float CalculateSecondAngle(const int second) const
	{
		return second * 6.0f; // 360도 / 60초 = 6도
	}

	vector<shared_ptr<Object>> objects;

	bool isColor = false;
};

