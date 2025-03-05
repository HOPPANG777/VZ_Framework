#pragma once

class TileMapManager
{
    DECLARE_SINGLETON(TileMapManager);

public:
    // 맵 데이터를 파일에 저장합니다.
    // 파일은 바이너리 형식으로 저장됩니다.
    // tileMap: 타일 객체들의 1차원 벡터 (row-major 순서)
    // rows, cols: 맵의 행과 열 수
    // fileName: 저장할 파일명 (예: "map01.dat")
    void SaveMap(const vector<shared_ptr<Tile>>& tileMap, int rows, int cols, const string& fileName);

    // 파일에서 맵 데이터를 불러옵니다.
    // outRows, outCols는 읽어들인 맵의 행/열 수를 반환합니다.
    // tileSize: 각 타일의 크기, tileSetPath: 타일셋 이미지 경로
    // tileSetColumns, tileSetRows: 타일셋 내 타일 수
    vector<shared_ptr<Tile>> LoadMap(const string& fileName, int& outRows, int& outCols, const Vector3& tileSize, const wstring& tileSetPath, UINT tileSetColumns, UINT tileSetRows);

};