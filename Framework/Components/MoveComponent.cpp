#include "Framework.h"
#include "MoveComponent.h"

void MoveComponent::Update()
{
	ResetKeyCount();

	if (Input::Get()->KeyPressed('W')) AddKeyCount();
	if (Input::Get()->KeyPressed('S')) AddKeyCount();
	if (Input::Get()->KeyPressed('A')) AddKeyCount();
	if (Input::Get()->KeyPressed('D')) AddKeyCount();

	if (keyCount == 2) speed = maxSpeed / sqrt(2.0f);
	else speed = maxSpeed;

	if (Input::Get()->KeyPressed('W')) owner->Move(Vector3(0, speed, 0));
	if (Input::Get()->KeyPressed('S')) owner->Move(Vector3(0, -speed, 0));
	if (Input::Get()->KeyPressed('A')) owner->Move(Vector3(-speed, 0, 0));
	if (Input::Get()->KeyPressed('D')) owner->Move(Vector3(speed, 0, 0));

	if (Input::Get()->KeyPressed(VK_DOWN)) owner->Scale(Vector3(0.5f) * -speed);
	if (Input::Get()->KeyPressed(VK_UP)) owner->Scale(Vector3(0.5f) * speed);

	if (Input::Get()->KeyPressed(VK_LEFT)) owner->Rotate(-speed);
	if (Input::Get()->KeyPressed(VK_RIGHT)) owner->Rotate(speed);
}

void MoveComponent::Render()
{

}
