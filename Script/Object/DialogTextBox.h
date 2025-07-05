#pragma once

class DialogTextBox : public Object
{
public:
    DialogTextBox(const string& name, const Vector3& position, const Vector3& texturePosition, const Vector3& scale, const float rotation, const wstring& backgroundTexturePath, const wstring& arrowTexturePath, const wstring& text, const float fontSize = 25.0f , const wstring& font = L"Pretendard JP", const Color& fontColor = BLACK);
    virtual ~DialogTextBox() = default;

public:
    virtual void Update() override;
    virtual void Render() override;
	virtual void PostRender();

public:
    // 대화창 활성/비활성 상태 제어
    void Activate() { active = true; }
    void Deactivate() { active = false; }

private:
    // 대화 텍스트
    shared_ptr<Object> background;
    shared_ptr<Object> arrowIndicator;
	shared_ptr<Object> playerTexture;

    // 배경, 텍스트, 화살표 아이콘 객체
    wstring dialogText;
	wstring fontName;
	Color fontColor;
    float fontSize;
    bool textComplete = false;
	bool arrowVisible = false;
    bool active = true;
};
