#pragma once

// 모든 에디터 패널들이 상속받는 인터페이스입니다.
// 각 패널은 ImGui 위젯을 통해 자신을 그립니다.
class IEditorPanel
{
public:
    virtual ~IEditorPanel() = default;

    // 패널 초기화
    virtual void Init() = 0;

    // 매 프레임 업데이트
    virtual void Update() = 0;

    // 렌더링 (필요할 경우)
    virtual void Render() = 0;

    // ImGui 위젯을 통한 UI 갱신
    virtual void ImGUI() = 0;
};

