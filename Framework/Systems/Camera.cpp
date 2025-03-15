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
    //UpdateFollow();
    //UpdateFollow2();
	UpdateView();
}

void Camera::Render()
{
	VPBuffer->SetVSBuffer(1);
}

void Camera::UpdateView()
{
	// 카메라가 보는 방향을 position에서 (0,0,1) 방향으로 설정
	view = XMMatrixLookAtLH
	(
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
        //position.x = target->GetPosition().x - WIN_DEFAULT_WIDTH * 0.5f;
        //position.y = target->GetPosition().y - WIN_DEFAULT_HEIGHT * 0.5f;
    }
    else // 타겟이 없을 경우 (개발자 모드)
    {
        if (INPUT->KeyDown(VK_F4))
        {
            // 토글: 이동 모드 On/Off
            isMove = !isMove;
            // 이동 모드 전환 시 가속 시간 초기화
            accelerationTime = 0.0f;
            currentSpeed = moveSpeed;
        }
        if (isMove)
        {
            bool moving = false;
            // 기본적으로 현재 속도(currentSpeed)를 사용하여 이동
            if (Keyboard::Get()->Press('W'))
            {
                position.y += currentSpeed * DELTA;
                moving = true;
            }
            else if (Keyboard::Get()->Press('S'))
            {
                position.y -= currentSpeed * DELTA;
                moving = true;
            }

            if (Keyboard::Get()->Press('A'))
            {
                position.x -= currentSpeed * DELTA;
                moving = true;
            }
            else if (Keyboard::Get()->Press('D'))
            {
                position.x += currentSpeed * DELTA;
                moving = true;
            }

            // 만약 한 개 이상의 이동 키가 눌려있으면 가속 누적, 아니면 가속 시간 초기화
            if (moving)
            {
                accelerationTime += DELTA;
                // 최소 5초 동안 누르고 있어야 최대 가속에 도달 (최대 가속은 5초에 해당)
                if (accelerationTime > 3.0f) accelerationTime = 3.0f;
            }
            else
            {
                accelerationTime = 0.0f;
            }
            // 가속 시간에 따라 속도를 선형적으로 증가 (예: 5초 후 2배 속도)
            currentSpeed = moveSpeed * (1.0f + accelerationTime / 5.0f);
        }
    }
}

const Vector3 Camera::Drag()
{
	Vector3 mousePos = INPUT->GetMousePos();

	mousePos += position;

	return mousePos;
}

void Camera::Reset()
{
	position = { 0, 0, 0 };  // 월드 좌표 (0, 0, 0)으로 초기화
	target = nullptr;        // 타겟 제거
	UpdateView();            // 초기화 후 뷰 갱신
}

void Camera::UpdateFollow()
{
    if (target == nullptr) return;

    // 플레이어(타겟)의 월드 좌표
    Vector3 targetPos = target->GetPosition();

    // 현재 카메라의 위치 (좌상단 기준)
    Vector3 camPos = position;

    // 데드 존 여백 설정 (원하는 값으로 조정 가능)
    float marginX = 600.0f;
    float marginY = 200.0f;

    // 현재 카메라 뷰포트 경계 계산
    float viewLeft = camPos.x;
    float viewRight = camPos.x + WIN_DEFAULT_WIDTH;
    float viewTop = camPos.y;
    float viewBottom = camPos.y + WIN_DEFAULT_HEIGHT;

    // 데드 존 내부 경계 (화면의 가장자리에서 margin 만큼 안쪽)
    float deadLeft = viewLeft + marginX;
    float deadRight = viewRight - marginX;
    float deadTop = viewTop + marginY;
    float deadBottom = viewBottom - marginY;

    // x축: 타겟이 데드 존 밖으로 벗어나면 카메라 x 위치 조정
    if (targetPos.x < deadLeft)
    {
        camPos.x = targetPos.x - marginX;
    }
    else if (targetPos.x > deadRight)
    {
        camPos.x = targetPos.x + marginX - WIN_DEFAULT_WIDTH;
    }

    // y축: 타겟이 데드 존 밖으로 벗어나면 카메라 y 위치 조정
    if (targetPos.y < deadTop)
    {
        camPos.y = targetPos.y - marginY;
    }
    else if (targetPos.y > deadBottom)
    {
        camPos.y = targetPos.y + marginY - WIN_DEFAULT_HEIGHT;
    }

    // 부드러운 이동을 원하면 보간(interpolation) 기법을 사용할 수 있음
    position = camPos;

    UpdateView();
}

void Camera::UpdateFollow2()
{
    if (target == nullptr) return;

    if (target->GetPosition().x > WIN_DEFAULT_WIDTH * 0.5 && target->GetPosition().x < WIN_DEFAULT_WIDTH * 1.5)
    {
        Vector3 newCamPos = Vector3(target->GetPosition().x - WIN_DEFAULT_WIDTH / 2, 0, 0);
        position = newCamPos;
        UpdateView();
    }
}



