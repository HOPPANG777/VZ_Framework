#pragma once

#include "Components/Component.h"

class EnemyAI : public Component
{
public:
	EnemyAI(const std::string& name = "EnemyAI") : Component(name) {}
	~EnemyAI() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

private:
	

};

