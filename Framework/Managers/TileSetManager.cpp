#include "Framework.h"
#include "TileSetManager.h"

TileSetManager::TileSetManager()
{
    Init();
}

TileSetManager::~TileSetManager()
{
    // ComPtr은 자동으로 Release()를 호출하므로 별도 처리 불필요
}

void TileSetManager::Init()
{
    // 기본 타일셋 경로 설정
    tilePath = TexturePath + L"tile.png";

    // Texture2D 객체를 unique_ptr로 생성
    auto tex = make_unique<Texture2D>(tilePath);
    // Texture2D 내부에서 ComPtr<ID3D11ShaderResourceView>를 관리하고 있다면 GetSRV()를 사용
    tileSRV = tex->GetSRV();
    // tex는 scope 종료 시 자동 삭제됨

    // GUI에서 표현할 타일셋의 가로, 세로 분할 개수
    tileXCount = 11;
    tileYCount = 7;

    texelTileSize = Vector2(1.0f / static_cast<float>(tileXCount), 1.0f / static_cast<float>(tileYCount));
}

void TileSetManager::ImGUI()
{
    ImGui::Begin("TileSetManager");
    {
        int count = 0;
        for (UINT y = 0; y < tileYCount; y++)
        {
            for (UINT x = 0; x < tileXCount; x++)
            {
                float startX = texelTileSize.x * x;
                float startY = texelTileSize.y * y;

                ImGui::PushID(count);
                // ImGui::ImageButton은 ImTextureID를 받으므로, tileSRV.Get()를 캐스팅
                if (ImGui::ImageButton("##TileButton",(ImTextureID)tileSRV.Get(), ImVec2(40, 40), ImVec2(startX, startY), ImVec2(startX + texelTileSize.x, startY + texelTileSize.y)))
                {
                    selectedStartUV = Vector2(startX, startY);
                }
                count++;
                ImGui::PopID();

                if (count % tileXCount != 0) ImGui::SameLine();
            }
        }
    }
    ImGui::End();
}

void TileSetManager::ChangeTileMap(const std::wstring& imagePath, UINT xCount, UINT yCount)
{
    tilePath = imagePath;

    auto tex = make_unique<Texture2D>(tilePath);
    tileSRV = tex->GetSRV();

    tileXCount = xCount;
    tileYCount = yCount;

    texelTileSize = Vector2(1.0f / static_cast<float>(tileXCount), 1.0f / static_cast<float>(tileYCount));
}