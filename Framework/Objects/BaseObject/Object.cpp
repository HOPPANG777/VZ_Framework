#include "Framework.h"
#include "Object.h"

void Object::AddComponent(const shared_ptr<Component>& component)
{
	if (components.count(component->GetName())) return;
	
	component->SetOwner(this);
	components.emplace(component->GetName(), component);
}

void Object::RemoveComponent(const string& compName)
{
	components.erase(compName);
}

void Object::Update()
{
	for (const auto& comp : components) comp.second->Update();

	//SRT
	{
		S = XMMatrixScalingFromVector(scale);
		R = XMMatrixRotationZ(-rotation);
		T = XMMatrixTranslationFromVector(position);
	}

	//world
	{
		world = S * R * T;
	}

	//worldBuffer
	{
		worldBuffer->SetWorld(world);
	}
}

void Object::Render()
{
	for (const auto& comp : components) comp.second->Render();

	//worldBuffer
	{
		worldBuffer->SetVSBuffer(0);
	}
}
