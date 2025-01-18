#include "Framework.h"
#include "Camera.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Init()
{
	VPBuffer = make_unique<ViewProjectionBuffer>();

	view = XMMatrixLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	proj = XMMatrixOrthographicOffCenterLH(0, WIN_DEFAULT_WIDTH, 0, WIN_DEFAULT_HEIGHT, 0, 1);

	VPBuffer->SetView(view);
	VPBuffer->SetProjection(proj);
}

void Camera::Update()
{
	Move();
	UpdateView();
}

void Camera::Render()
{
	VPBuffer->SetVSBuffer(1);
}

void Camera::UpdateView()
{
	// 카메라가 보는 방향을 position에서 (0,0,1) 방향으로 설정
	view = XMMatrixLookAtLH(
		position,                      // 카메라 위치
		position + Vector3(0, 0, 1),   // 카메라가 보는 방향
		Vector3(0, 1, 0)               // 카메라의 위쪽 방향
	);

	VPBuffer->SetView(view);
}

void Camera::Move()
{
	// 타겟이 있을 경우
	if (target != nullptr)
	{
		position.x = target->GetPosition().x - WIN_DEFAULT_WIDTH * 0.5f;
		position.y = target->GetPosition().y - WIN_DEFAULT_HEIGHT * 0.5f;
	}
	else // 타겟이 없을경우 // 개발자 모드
	{
		if (KEYBOARD->Down(VK_F4))
		{
			// 참은 거짓으로 거짓은 참으로
			isMove = !isMove;
		}
		if (isMove)
		{
			if (Keyboard::Get()->Press('W'))
				position.y += moveSpeed * DELTA;
			else if (Keyboard::Get()->Press('S'))
				position.y -= moveSpeed * DELTA;

			if (Keyboard::Get()->Press('A'))
				position.x -= moveSpeed * DELTA;
			else if (Keyboard::Get()->Press('D'))
				position.x += moveSpeed * DELTA;
		}
	}
}

const Vector3 Camera::Drag()
{
	Vector3 mousePos = MOUSE->GetMousePos();

	mousePos += position;

	return mousePos;
}

void Camera::Reset()
{
	position = { 0, 0, 0 };  // 월드 좌표 (0, 0, 0)으로 초기화
	target = nullptr;        // 타겟 제거
	UpdateView();            // 초기화 후 뷰 갱신
}


