#include "Framework.h"
#include "MoveComponent.h"

void MoveComponent::Update()
{
	ResetKeyCount();

	if (INPUT->KeyPressed('W')) AddKeyCount();
	if (INPUT->KeyPressed('S')) AddKeyCount();
	if (INPUT->KeyPressed('A')) AddKeyCount();
	if (INPUT->KeyPressed('D')) AddKeyCount();

	if (keyCount == 2) speed = maxSpeed / sqrt(2.0f);
	else speed = maxSpeed;

	if (INPUT->KeyPressed('W')) owner->Move(Vector3(0, speed, 0));
	if (INPUT->KeyPressed('S')) owner->Move(Vector3(0, -speed, 0));
	if (INPUT->KeyPressed('A')) owner->Move(Vector3(-speed, 0, 0));
	if (INPUT->KeyPressed('D')) owner->Move(Vector3(speed, 0, 0));

	if (INPUT->KeyPressed(VK_DOWN)) owner->Scale(Vector3(0.5f) * -speed);
	if (INPUT->KeyPressed(VK_UP)) owner->Scale(Vector3(0.5f) * speed);

	if (INPUT->KeyPressed(VK_LEFT)) owner->Rotate(-speed);
	if (INPUT->KeyPressed(VK_RIGHT)) owner->Rotate(speed);
}

void MoveComponent::Render()
{

}
