#pragma once

#include <cstdint>

struct Vector3;

class IInput
{
public:
    virtual ~IInput() = default;

    virtual void Update() = 0;
    virtual bool KeyDown(uint32_t key) const = 0;
    virtual bool KeyUp(uint32_t key) const = 0;
    virtual bool KeyPressed(uint32_t key) const = 0;

    virtual Vector3 GetMousePos() const = 0;
    virtual bool MouseDown(uint32_t button) const = 0;
    virtual bool MouseUp(uint32_t button) const = 0;
    virtual bool MousePressed(uint32_t button) const = 0;
    virtual Vector3 GetMouseMove() const = 0;
};
