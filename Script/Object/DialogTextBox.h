#pragma once

class DialogTextBox : public Object
{
public:
    DialogTextBox(const string& name, const Vector3& position, const Vector3& size, const wstring& backgroundTexturePath, const wstring& arrowTexturePath);
    virtual ~DialogTextBox() = default;

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    // 대화 텍스트 설정
    void SetDialogText(const wstring& text);

public:
    // 대화창 활성/비활성 상태 제어
    void Activate() { active = true; }
    void Deactivate() { active = false; }
    
private:
    // 간단한 헬퍼 함수: 2D 좌표와 RECT의 겹침 여부 판정 (z는 무시)
    static bool PointInRect(const Vector3& point, const RECT& rect)
    {
        return (point.x >= rect.left && point.x <= rect.right &&
            point.y >= rect.top && point.y <= rect.bottom);
    }

private:
    // 배경, 텍스트, 화살표 아이콘 객체
    shared_ptr<TextureRect> background;
    shared_ptr<Object> textObject;
    shared_ptr<TextureRect> arrowIndicator;

    // 대화 텍스트
    wstring dialogText;
    bool textComplete = false;
    bool active = true;
};
