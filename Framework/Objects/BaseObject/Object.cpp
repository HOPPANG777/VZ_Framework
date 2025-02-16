#include "Framework.h"
#include "Object.h"

Object::Object(const string& name)
    : name(name)
{
    worldBuffer = make_unique<WorldBuffer>();

    // 모든 Object는 Transform을 무조건 포함
    auto transform = make_shared<Transform>("Transform");
    AddComponent(transform);
}

Object::Object(const string& name, const Vector3& initPos, const Vector3& initScale, float initRot)
    : Object(name) // 위 기본 생성자 호출 후, Transform 초기값 세팅
{
    auto transform = GetComponent<Transform>("Transform");
    if (transform)
    {
        transform->SetPosition(initPos);
        transform->SetScale(initScale);
        transform->SetRotation(initRot);
    }
}

Object::Object(const Object& other)
    : Object(other.name)
{
    // 복사 생성자 로직이 필요하면 작성
    // 여기서는 간단히 name만 복사 예시
    // 만약 컴포넌트를 복사해야 한다면
    // (componets 맵을 돌며 clone 형태를 구현) 추가 작업이 필요
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

shared_ptr<Transform> Object::GetTransformComponent() const
{
    return GetComponent<Transform>("Transform");
}

shared_ptr<RectTransform> Object::GetRectTransformComponent() const
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