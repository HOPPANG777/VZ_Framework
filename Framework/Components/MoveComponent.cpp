#include "Framework.h"
#include "MoveComponent.h"

void MoveComponent::Update()
{
	ResetKeyCount();

	if (KEYBOARD->Press('W')) AddKeyCount();
	if (KEYBOARD->Press('S')) AddKeyCount();
	if (KEYBOARD->Press('A')) AddKeyCount();
	if (KEYBOARD->Press('D')) AddKeyCount();

	if (keyCount == 2) speed = maxSpeed / sqrt(2.0f);
	else speed = maxSpeed;

	if (KEYBOARD->Press('W')) owner->Move(Vector3(0, speed, 0));
	if (KEYBOARD->Press('S')) owner->Move(Vector3(0, -speed, 0));
	if (KEYBOARD->Press('A')) owner->Move(Vector3(-speed, 0, 0));
	if (KEYBOARD->Press('D')) owner->Move(Vector3(speed, 0, 0));

	if (KEYBOARD->Press(VK_DOWN)) owner->Scale(Vector3(0.5f) * -speed);
	if (KEYBOARD->Press(VK_UP)) owner->Scale(Vector3(0.5f) * speed);

	if (KEYBOARD->Press(VK_LEFT)) owner->Rotate(-speed);
	if (KEYBOARD->Press(VK_RIGHT)) owner->Rotate(speed);
}

void MoveComponent::Render()
{

}
