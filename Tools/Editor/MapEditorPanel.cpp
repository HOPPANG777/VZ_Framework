#include "stdafx.h"
#include "MapEditorPanel.h"
#include "Managers/TileSetManager.h"
#include "Utilities/String.h"

MapEditorPanel::~MapEditorPanel()
{
    if (tmap)
        tmap.reset();
}

void MapEditorPanel::Init()
{
    // 초기화 시 특별한 동작은 필요하지 않음
        // 맵 편집에 사용할 카메라 초기화
    previewCamera = Camera::Get();
    previewCamera->Init();
}

void MapEditorPanel::Update()
{
    // 카메라 이동 및 기타 업데이트 처리
    if (previewCamera)
        previewCamera->Update();

    // ImGui에서 마우스를 사용 중이라면 편집 로직을 건너뜁니다
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (tmap)
        tmap->Update();
}

void MapEditorPanel::Render()
{
    if (previewCamera)
        previewCamera->Render();

    if (tmap)
        tmap->Render();
}

void MapEditorPanel::ImGUI()
{
    ImGui::Begin("Map Editor");
    {
        ImGui::InputInt("Spacing", reinterpret_cast<int*>(&spacing));
        width = (Values::GetWinWidth() * 2) / spacing;
        height = (Values::GetWinHeight() * 2) / spacing;
        ImGui::Text("Calculated Map Width: %d", width);
        ImGui::Text("Calculated Map Height: %d", height);

        static char fileNameBuffer[128] = "map01";
        ImGui::InputText("Map File Name", fileNameBuffer, IM_ARRAYSIZE(fileNameBuffer));

        if (ImGui::Button("Create New Map"))
            CreateDefaultMap();
        ImGui::SameLine();
        if (ImGui::Button("Save Map"))
        {
            std::string fullPath = std::string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
            std::string fulltextPath = std::string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".txt";
            if (tmap)
            {
                tmap->Save(fullPath);
                tmap->Save(fulltextPath);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map"))
        {
            tmap = std::make_unique<TileMap>(width, height, spacing, true);
            std::string fullPath = std::string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
            tmap->Load(fullPath);
        }

        static int newMapWidth = width;
        static int newMapHeight = height;
        ImGui::InputInt("New Map Width", &newMapWidth);
        ImGui::InputInt("New Map Height", &newMapHeight);
        if (ImGui::Button("Resize Map"))
        {
            if (tmap)
                tmap->ResizeMap(newMapWidth, newMapHeight);
        }

        if (ImGui::CollapsingHeader("Tileset Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static char tileSetNameBuffer[256] = "MyTileSet.png";
            ImGui::InputText("Tileset Name", tileSetNameBuffer, IM_ARRAYSIZE(tileSetNameBuffer));

            static int tileSetColumns = TileSetManager::Get()->GetXCount();
            static int tileSetRows = TileSetManager::Get()->GetYCount();
            ImGui::InputInt("Tileset Columns", &tileSetColumns);
            ImGui::InputInt("Tileset Rows", &tileSetRows);

            if (ImGui::Button("Apply Tileset"))
            {
                std::wstring tileSetName = String::ToWstring(std::string(tileSetNameBuffer));
                std::wstring newTilePath = TexturePath + L"Tilesets/" + tileSetName + L".png";
                TileSetManager::Get()->ChangeTileMap(newTilePath, tileSetColumns, tileSetRows);
            }
        }

        if (tmap)
            tmap->ImGUI();
    }
    ImGui::End();
}

void MapEditorPanel::CreateDefaultMap()
{
    tmap = std::make_unique<TileMap>(width, height, spacing, true);
}

