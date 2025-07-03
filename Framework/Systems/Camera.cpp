#include "Framework.h"
#include "Camera.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Init()
{
	VPBuffer = std::make_unique<ViewProjectionBuffer>();

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
        position.x = target->GetPosition().x - WIN_DEFAULT_WIDTH * 0.5f;
        position.y = target->GetPosition().y - WIN_DEFAULT_HEIGHT * 0.5f;
    }
    else // 타겟이 없을 경우 (개발자 모드)
    {
        if (Input::Get()->KeyDown(VK_F4))
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
            if (Input::Get()->KeyPressed('W'))
            {
                position.y += currentSpeed * Time::Get()->GetDeltaTime();
                moving = true;
            }
            else if (Input::Get()->KeyPressed('S'))
            {
                position.y -= currentSpeed * Time::Get()->GetDeltaTime();
                moving = true;
            }

            if (Input::Get()->KeyPressed('A'))
            {
                position.x -= currentSpeed * Time::Get()->GetDeltaTime();
                moving = true;
            }
            else if (Input::Get()->KeyPressed('D'))
            {
                position.x += currentSpeed * Time::Get()->GetDeltaTime();
                moving = true;
            }

            // 만약 한 개 이상의 이동 키가 눌려있으면 가속 누적, 아니면 가속 시간 초기화
            if (moving)
            {
                accelerationTime += Time::Get()->GetDeltaTime();
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
	Vector3 mousePos = Input::Get()->GetMousePos();

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

    // (1) 현재 카메라의 좌측 상단을 camPos로 가정
    Vector3 camPos = position;
    Vector3 targetPos = target->GetPosition();

    // (2) 데드존(marginX, marginY) 밖이면 카메라를 이동
    float marginX = 600.0f;
    float marginY = 200.0f;

    float viewLeft = camPos.x;
    float viewRight = camPos.x + WIN_DEFAULT_WIDTH;
    float viewTop = camPos.y;
    float viewBottom = camPos.y + WIN_DEFAULT_HEIGHT;

    float deadLeft = viewLeft + marginX;
    float deadRight = viewRight - marginX;
    float deadTop = viewTop + marginY;
    float deadBottom = viewBottom - marginY;

    // x축
    if (targetPos.x < deadLeft) camPos.x = targetPos.x - marginX;
    else if (targetPos.x > deadRight) camPos.x = targetPos.x + marginX - WIN_DEFAULT_WIDTH;

    // y축
    if (targetPos.y < deadTop) camPos.y = targetPos.y - marginY;
    else if (targetPos.y > deadBottom) camPos.y = targetPos.y + marginY - WIN_DEFAULT_HEIGHT;

    // 최종 카메라 위치 갱신
    position = camPos;

    // 뷰 행렬 업데이트
    UpdateView();
}

void Camera::UpdateHorizontalScroll(Object* target, float minRatio, float maxRatio)
{
    if (target == nullptr) return;

    // 타겟의 월드 좌표
    Vector3 targetPos = target->GetPosition();

    // x와 y 모두 각각의 조건 범위 내에 있을 경우에만 업데이트
    if (targetPos.x > WIN_DEFAULT_WIDTH * minRatio && targetPos.x < WIN_DEFAULT_WIDTH * maxRatio)
    {
        // target의 중앙이 화면 중앙에 오도록 카메라 위치 설정
        float newX = targetPos.x - WIN_DEFAULT_WIDTH / 2.0f;

        SetPosition(Vector3(newX, position.y, position.z));
    }

}

void Camera::UpdateVerticalScroll(Object* target, float minRatio, float maxRatio)
{
    if (target == nullptr) return;

    // 타겟의 월드 좌표
    Vector3 targetPos = target->GetPosition();

    if (targetPos.y > WIN_DEFAULT_HEIGHT * minRatio && targetPos.y < WIN_DEFAULT_HEIGHT * maxRatio)
    {
        float newY = targetPos.y - WIN_DEFAULT_HEIGHT / 2.0f;

        SetPosition(Vector3(position.x, newY, position.z));
    }
}



