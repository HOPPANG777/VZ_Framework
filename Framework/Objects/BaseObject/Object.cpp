#include "Framework.h"
#include "Object.h"

Object::Object(const string& name, TransformType type)
    : name(name)
{
    worldBuffer = make_unique<WorldBuffer>();

    // 선택에 따라 적절한 Transform 컴포넌트 추가
    if (type == TransformType::Normal)
    {
        auto transform = make_shared<Transform>("Transform");
        AddComponent(transform);
    }
    else if (type == TransformType::Rect)
    {
        auto rectTransform = make_shared<RectTransform>("RectTransform");
        AddComponent(rectTransform);
    }
}

Object::Object(const string& name, const Vector3& initPos, const Vector3& initScale, float initRot, TransformType type)
    : Object(name, type)
{
    // 타입에 따라 초기값 설정
    if (type == TransformType::Normal)
    {
        auto transform = GetComponent<Transform>("Transform");
        if (transform)
        {
            transform->SetPosition(initPos);
            transform->SetScale(initScale);
            transform->SetRotation(initRot);
        }
    }
    else if (type == TransformType::Rect)
    {
        auto rectTransform = GetComponent<RectTransform>("RectTransform");
        if (rectTransform)
        {
            rectTransform->SetPosition(initPos);
            rectTransform->SetScale(initScale);
            rectTransform->SetRotation(initRot);
        }
    }
}

Object::Object(const Object& other)
    : Object(other.name, TransformType::Normal) // 복사 생성자 로직은 필요에 따라 수정
{
    // 복사 생성자 구현 (필요 시 components 복사)
}

void Object::AddComponent(const shared_ptr<Component>& component)
{
    if (components.count(component->GetName())) return;

    component->SetOwner(this);
    components.emplace(component->GetName(), component);
}

void Object::RemoveComponent(const std::string& compName)
{
    components.erase(compName);
}

shared_ptr<Transform> Object::GetTransform() const
{
    return GetComponent<Transform>("Transform");
}

shared_ptr<RectTransform> Object::GetRectTransform() const
{
    return GetComponent<RectTransform>("RectTransform");
}

void Object::Update()
{
    // 등록된 모든 컴포넌트 업데이트
    for (auto& it : components)
    {
        // it은 map< string, shared_ptr<Component> >::value_type &
        auto& compName = it.first;
        auto& compPtr = it.second;

        compPtr->Update();
    }

    // RectTransform이 있다면 우선적으로 사용합니다.
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        worldBuffer->SetWorld(rectTrans->GetWorld());
    }
    else
    {
        auto trans = GetComponent<Transform>("Transform");
        if (trans) worldBuffer->SetWorld(trans->GetWorld());
    }
}

void Object::Render()
{
    // 등록된 모든 컴포넌트 Render
    for (auto& it : components)
    {
        // it은 map< string, shared_ptr<Component> >::value_type &
        auto& compName = it.first;
        auto& compPtr = it.second;

        compPtr->Render();
    }

    // WorldBuffer를 VS에 등록 (셰이더에서 월드 행렬 사용 가능)
    worldBuffer->SetVSBuffer(0);
}

// ------------------------------------------------------
// Transform 접근 편의 함수들
// ------------------------------------------------------
Vector3 Object::GetPosition() const
{
    // RectTransform이 있으면 그것을 사용하고, 없으면 기본 Transform 사용
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)return rectTrans->GetPosition();

    auto trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetPosition() : Vector3(0, 0, 0);
}

Vector3 Object::GetScale() const
{
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetScale();

    auto trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetScale() : Vector3(1, 1, 1);
}

float Object::GetRotation() const
{
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetRotation();

    auto trans = GetComponent<Transform>("Transform");
    return trans ? trans->GetRotation() : 0.0f;
}

RECT Object::GetRect()
{
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans) return rectTrans->GetRect();
}

void Object::Move(const Vector3& speed)
{
    // 상황에 따라 RectTransform이나 Transform을 선택적으로 업데이트할 수 있습니다.
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Move(speed);
    }
    else
    {
        auto trans = GetComponent<Transform>("Transform");
        if (trans) trans->Move(speed);
    }
}

void Object::Rotate(float rot)
{
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Rotate(rot);
    }
    else
    {
        auto trans = GetComponent<Transform>("Transform");
        if (trans) trans->Rotate(rot);
    }
}

void Object::Scale(const Vector3& scaleChange)
{
    auto rectTrans = GetComponent<RectTransform>("RectTransform");
    if (rectTrans)
    {
        rectTrans->Scale(scaleChange);
    }
    else
    {
        auto trans = GetComponent<Transform>("Transform");
        if (trans) trans->Scale(scaleChange);
    }
}