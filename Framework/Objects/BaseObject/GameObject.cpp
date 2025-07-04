#include "Framework.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name, TransformType type)
    : name(name)
{
    worldBuffer = std::make_unique<WorldBuffer>();

    // 선택에 따라 적절한 Transform 컴포넌트 추가
    if (type == TransformType::Normal)
    {
        const auto& transform = std::make_shared<Transform>("Transform");
        AddComponent(transform);
    }
    else if (type == TransformType::Rect)
    {
        const auto& rectTransform = std::make_shared<RectTransform>("RectTransform");
        AddComponent(rectTransform);
    }
}

GameObject::GameObject(const std::string& name, const Vector3& initPos, const Vector3& initScale, float initRot, TransformType type)
    : GameObject(name, type)
{
    // 타입에 따라 초기값 설정
    if (type == TransformType::Normal)
    {
        const auto& transform = GetComponent<Transform>("Transform");
        if (transform)
        {
            transform->SetPosition(initPos);
            transform->SetScale(initScale);
            transform->SetRotation(initRot);
        }
    }
    else if (type == TransformType::Rect)
    {
        const auto& rectTransform = GetComponent<RectTransform>("RectTransform");
        if (rectTransform)
        {
            rectTransform->SetPosition(initPos);
            rectTransform->SetScale(initScale);
            rectTransform->SetRotation(initRot);
        }
    }
}

GameObject::GameObject(const GameObject& other)
    : GameObject(other.name, TransformType::Normal) // 복사 생성자 로직은 필요에 따라 수정
{
    // 복사 생성자 구현 (필요 시 components 복사)
}

void GameObject::AddComponent(const std::shared_ptr<Component>& component)
{
    if (components.count(component->GetName())) return;

    component->SetOwner(this);
    components.emplace(component->GetName(), component);
}

void GameObject::RemoveComponent(const std::string& compName)
{
    components.erase(compName);
}

std::shared_ptr<Transform> GameObject::GetTransform() const
{
    return GetComponent<Transform>("Transform");
}

std::shared_ptr<RectTransform> GameObject::GetRectTransform() const
{
    return GetComponent<RectTransform>("RectTransform");
}

void GameObject::Update()
{
    // 등록된 모든 컴포넌트 업데이트
    // 여기서 each element의 타입은 std::pair<const std::string, std::shared_ptr<Component>>
    for (const auto& element : components)
    {
        const auto& compName = element.first;    // 컴포넌트 이름
        const auto& compPtr = element.second; // 컴포넌트 포인터

        compPtr->Update();
    }

    // RectTransform이 있다면 우선적으로 사용합니다.
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        worldBuffer->SetWorld(rectTrans->GetWorld());
    }
    else
    {
        const auto& trans = GetComponent<Transform>("Transform");
        if (trans) 
        {
            worldBuffer->SetWorld(trans->GetWorld());
        }
    }
}

void GameObject::Render()
{
    // 등록된 모든 컴포넌트 Render
    for (const auto& element : components)
    {
        const auto& compName = element.first;
        const auto& compPtr = element.second;

        compPtr->Render();
    }

    // WorldBuffer를 VS에 등록 (셰이더에서 월드 행렬 사용 가능)
    worldBuffer->SetVSBuffer(0);
}

// ------------------------------------------------------
// Transform 접근 편의 함수들
// ------------------------------------------------------
Vector3 GameObject::GetPosition() const
{
    // RectTransform이 있으면 그것을 사용하고, 없으면 기본 Transform 사용
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)return rectTrans->GetPosition();

    const auto& trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetPosition() : Vector3(0, 0, 0);
}

Vector3 GameObject::GetScale() const
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetScale();

    const auto& trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetScale() : Vector3(1, 1, 1);
}

float GameObject::GetRotation() const
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetRotation();

    const auto& trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetRotation() : 0.0f;
}

void GameObject::SetPosition(const Vector3& position)
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->SetPosition(position);

    const auto& trans = GetComponent<Transform>("Transform");
    return trans->SetPosition(position);
}

void GameObject::SetScale(const Vector3& scale)
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->SetScale(scale);

    const auto& trans = GetComponent<Transform>("Transform");
    return trans->SetScale(scale);
}

void GameObject::SetRotation(const float& rotation)
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->SetRotation(rotation);

    const auto& trans = GetComponent<Transform>("Transform");
    return trans->SetRotation(rotation);
}

RECT GameObject::GetRect()
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetRect();
}

RECT GameObject::GetCorrectedRect()
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetCorrectedRect();
}

void GameObject::Move(const Vector3& speed)
{
    // 상황에 따라 RectTransform이나 Transform을 선택적으로 업데이트할 수 있습니다.
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Move(speed);
    }
    else
    {
        const auto& trans = GetComponent<Transform>("Transform");
        if (trans) trans->Move(speed);
    }
}

void GameObject::Rotate(float rot)
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Rotate(rot);
    }
    else
    {
        const auto& trans = GetComponent<Transform>("Transform");
        if (trans) trans->Rotate(rot);
    }
}

void GameObject::Scale(const Vector3& scaleChange)
{
    const auto& rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Scale(scaleChange);
    }
    else
    {
        const auto& trans = GetComponent<Transform>("Transform");
        if (trans) trans->Scale(scaleChange);
    }
}