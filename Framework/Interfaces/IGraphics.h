#pragma once

class IGraphics
{
public:
    virtual ~IGraphics() = default;

    virtual void Init() = 0;
    virtual void CreateBackBuffer() = 0;
    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void SetFullScreen(bool fullscreen) = 0;
    virtual void SetResolution(unsigned int width, unsigned int height) = 0;

    virtual bool LoadCustomFont(const std::wstring& fontFilePath) = 0;
    virtual bool UnloadCustomFont(const std::wstring& fontFilePath) = 0;
};
