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

    // Transform의 월드 행렬을 얻어와서 worldBuffer에 세팅
    auto transform = GetComponent<Transform>("Transform");
    if (transform)
    {
        worldBuffer->SetWorld(transform->GetWorld());
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
    auto transform = GetComponent<Transform>("Transform");
    return transform ? transform->GetPosition() : Vector3(0, 0, 0);
}

Vector3 Object::GetScale() const
{
    auto transform = GetComponent<Transform>("Transform");
    return transform ? transform->GetScale() : Vector3(1, 1, 1);
}

float Object::GetRotation() const
{
    auto transform = GetComponent<Transform>("Transform");
    return transform ? transform->GetRotation() : 0.0f;
}

void Object::Move(const Vector3& speed)
{
    auto transform = GetComponent<Transform>("Transform");
    if (transform) transform->Move(speed);
}

void Object::Rotate(float rot)
{
    auto transform = GetComponent<Transform>("Transform");
    if (transform) transform->Rotate(rot);
}

void Object::Scale(const Vector3& scale)
{
    auto transform = GetComponent<Transform>("Transform");
    if (transform) transform->Scale(scale);
}