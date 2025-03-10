#pragma once

class Tile  
{
private:
    Vector3 position = Values::ZeroVec3;
    Color color = Values::Black;

    Vector2 startUV = Values::ZeroVec2;
    Vector2 endUV = Values::ZeroVec2;

    ComPtr<ID3D11ShaderResourceView> srv; // D3D 府家胶绰 ComPtr肺 包府

    UINT index = -1;

public:
    // Getter/Setter
    Vector3 GetPosition() const { return position; }
    void SetPosition(const Vector3& position) { this->position = position; }

    Vector2 GetStartUV() const { return startUV; }
    void SetStartUV(const Vector2& uv) { this->startUV = uv; }

    Vector2 GetEndUV() const { return endUV; }
    void SetEndUV(const Vector2& uv) { this->endUV = uv; }

    Color GetColor() const { return color; }
    void SetColor(const Color& color) { this->color = color; }

    UINT GetIndex() const { return index; }
    void SetIndex(UINT index) { this->index = index; }

public:
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }
    void SetSRV(const ComPtr<ID3D11ShaderResourceView>& srv) { this->srv = srv; }

};