#pragma once

class IColorable
{
public:
    virtual ~IColorable() = default;
    virtual void SetColor(const Color& color) = 0;
    virtual Color GetColor() const = 0;

};