#pragma once


class BlinkComponent : public Component
{
public:
    BlinkComponent(const string& name = "BlinkComponent") : Component(name) {}
    virtual ~BlinkComponent() override = default;

    void Activate(float duration = 1.0f, float interval = 0.2f);

    virtual void Update() override;
    virtual void Render() override {}

private:
    bool active = false;
    float timer = 0.0f;
    float duration = 0.0f;
    float interval = 0.2f;
    float tick = 0.0f;
    bool visible = true;
    Color originalColor = WHITE;
};

