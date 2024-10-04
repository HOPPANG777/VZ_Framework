#include "Framework.h"
#include "MoveComponent.h"

void MoveComponent::Update()
{
	ResetKeyCount();

	if (INPUT->Press('W')) AddKeyCount();
	if (INPUT->Press('S')) AddKeyCount();
	if (INPUT->Press('A')) AddKeyCount();
	if (INPUT->Press('D')) AddKeyCount();

	if (keyCount == 2) speed = maxSpeed / sqrt(2.0f);
	else speed = maxSpeed;

	if (INPUT->Press('W')) owner->Move(Vector2(0, speed));
	if (INPUT->Press('S')) owner->Move(Vector2(0, -speed));
	if (INPUT->Press('A')) owner->Move(Vector2(-speed, 0));
	if (INPUT->Press('D')) owner->Move(Vector2(speed, 0));

	if (INPUT->Press(VK_DOWN)) owner->Scale(Vector2(0.5f) * -speed);
	if (INPUT->Press(VK_UP)) owner->Scale(Vector2(0.5f) * speed);

	if (INPUT->Press(VK_LEFT)) owner->Rotate(-speed);
	if (INPUT->Press(VK_RIGHT)) owner->Rotate(speed);
}

void MoveComponent::Render()
{

}
