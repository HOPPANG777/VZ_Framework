#include "Framework.h"
#include "TileSetManager.h"

TileSetManager::TileSetManager()
{
    Init();
}

TileSetManager::~TileSetManager() {}

void TileSetManager::ImGUI()
{
    ImGui::Begin("TileSet");
    ImGui::Text("Select Tile:");
    ImGui::PushID("TileSetImage");
    ImGui::Image((ImTextureID)tileSRV.Get(), ImVec2(256, 256));
    if (ImGui::IsItemClicked())
    {
        ImVec2 imagePos = ImGui::GetItemRectMin();
        ImVec2 mousePos = ImGui::GetMousePos();
        float relX = mousePos.x - imagePos.x;
        float relY = mousePos.y - imagePos.y;
        float tileWidth = 256.0f / tileXCount;
        float tileHeight = 256.0f / tileYCount;
        int col = static_cast<int>(relX / tileWidth);
        int row = static_cast<int>(relY / tileHeight);
        selectedStartUV = Vector2(col * texelTileSize.x, row * texelTileSize.y);
    }
    ImGui::PopID();
    ImGui::Text("Tile: (%.2f, %.2f)", selectedStartUV.x, selectedStartUV.y);
    ImGui::End();
}

void TileSetManager::ChangeTileMap(const wstring& imagePath, const UINT& xCount, const UINT& yCount)
{
    tilePath = imagePath;

    auto tex = make_unique<Texture2D>(tilePath);
    tileSRV = tex->GetSRV();

    tileXCount = xCount;
    tileYCount = yCount;

    texelTileSize = Vector2(1.0f / tileXCount, 1.0f / tileYCount);
}

void TileSetManager::Init()
{
    tilePath = TexturePath + L"tile.png";

    auto tex = make_unique<Texture2D>(tilePath);
    tileSRV = tex->GetSRV();

    tileXCount = 11;
    tileYCount = 7;

    texelTileSize = Vector2(1.0f / tileXCount, 1.0f / tileYCount);
}
