#include "Framework.h"
#include "TileMap.h"

TileMap::TileMap(UINT width, UINT height, UINT spacing, bool editNow)
	: width(width), height(height), spacing(spacing), editNow(editNow)
{
	// 타일셋(이미지) 관련 설정은 TileSetManager 싱글톤을 사용
	// 타일셋 매니저는 내부에서 텍스처(SRV)와 타일 분할 정보를 관리합니다.
	//TILESET->Init();

	// 타일맵 생성 (2차원 vector 형태)
	GenerateTileMap();

	//vertices
	{
		// 타일 하나를 그리기 위한 기본 버텍스 데이터 (사각형)
		//vertices.resize(4);
		vertices.assign(4, VertexTile());

		vertices[0].position = Values::ZeroVec3;
		vertices[1].position = Vector3(0.0f, (float)spacing, 0.0f);
		vertices[2].position = Vector3((float)spacing, 0.0f, 0.0f);
		vertices[3].position = Vector3((float)spacing, (float)spacing, 0.0f);
	}

	//indices
	{
		// 인덱스 데이터 (사각형 2개의 삼각형)
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//vertexBuffer
	{
		// D3D 리소스 생성 (Framework의 매크로나 unique_ptr 활용)
		vertexBuffer = make_unique<VertexBuffer>();
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indexBuffer
	{
		indexBuffer = make_unique<IndexBuffer>();
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//vertexBuffer
	{
		vertexShader = make_unique<VertexShader>();
		vertexShader->Create(ShaderPath + L"VertexTile.hlsl", "VS");
	}

	//pixelShader
	{
		pixelShader = make_unique<PixelShader>();
		pixelShader->Create(ShaderPath + L"VertexTile.hlsl", "PS");
	}

	//inputLayout
	{
		inputLayout = make_unique<InputLayout>();
		inputLayout->Create(VertexTile::isDescs.data(), VertexTile::isDescs.size(), vertexShader->GetBlob());
	}

	//worldBuffer
	{
		worldBuffer = make_unique<WorldBuffer>();
		worldBuffer->SetWorld(world);
	}

	//indexNumBuffer
	{
		indexNumBuffer = make_unique<IndexNumBuffer>();
		indexNumBuffer->SetIndex(0);
	}

	// Rasterizer State
	// 레스터라이저는 버텍스 쉐이더로 찍은 정점들의 사이와 내부를
	// 채워주는 역할이다.
	// 이거 안만들면 기본값으로 넣어주고 만들면 만든거 넣어줌
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		// CullMode : 그리지 않을 부분 설정
		// d3d11_cull_back == 뒷면 지우겠다. (반시계 방향 안그림)
		desc.CullMode = D3D11_CULL_BACK;
		//desc.CullMode = D3D11_CULL_NONE;

		// FillMode : 체우는 방식 설정
		// d3d11_fill_solid == 전부 체우겠다. (빈공간 안만든다)
		desc.FillMode = D3D11_FILL_SOLID;

		// 시계방향 설정 (false == 시계 방향 // true == 반 시계 방향)
		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, rs.GetAddressOf());
		CHECK(hr);
	}

	// Sampler
	{
		D3D11_SAMPLER_DESC desc;
		RenderStates::GetSamplerDesc(&desc);
		RenderStates::CreateSampler(&desc, sampler[0].GetAddressOf());

		// 선형보간
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		RenderStates::CreateSampler(&desc, sampler[1].GetAddressOf());
	}

	// Blend
	{
		D3D11_BLEND_DESC desc;
		RenderStates::GetBlendDesc(&desc);
		RenderStates::CreateBlend(&desc, blend[0].GetAddressOf());

		desc.RenderTarget[0].BlendEnable = true;
		RenderStates::CreateBlend(&desc, blend[1].GetAddressOf());
	}
}

TileMap::~TileMap()
{
	for (auto& tile : tiles)
	{
		for (auto& t : tile)
		{
			t.reset();
		}
	}
}

void TileMap::GenerateTileMap()
{
	if (width == 0 || height == 0 || spacing == 0) assert(false);

	// 2차원 벡터를 사용하여 각 행에 대해 vector<shared_ptr<Tile>>를 생성합니다.
	//vector<vector<shared_ptr<Tile>>> tiles;
	tiles.resize(height);

	int index = 0;
	for (UINT y = 0; y < height; y++)
	{
		// 각 행의 열 크기를 미리 할당합니다.
		tiles[y].resize(width);
		for (UINT x = 0; x < width; x++)
		{
			// 각 타일을 make_shared로 생성합니다.
			tiles[y][x] = make_shared<Tile>();
			tiles[y][x]->SetPosition(Vector3(static_cast<float>(x * spacing), static_cast<float>(y * spacing), 0.0f));
			tiles[y][x]->SetIndex(index++);
		}
	}
}

void TileMap::MapVertexBuffer()
{
	DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TileMap::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	DC->Unmap(vertexBuffer->GetResource(), 0);
}

void TileMap::ResizeMap(UINT newWidth, UINT newHeight)
{
    // 새로운 2차원 벡터 준비
    vector<vector<shared_ptr<Tile>>> newTiles;
    newTiles.resize(newHeight);

    // 인덱스를 새로 부여할 때 사용할 변수
    int index = 0;

    for (UINT y = 0; y < newHeight; y++)
    {
        newTiles[y].resize(newWidth);

        for (UINT x = 0; x < newWidth; x++)
        {
            if (x < width && y < height)
            {
                // 기존 맵 범위 안에 있으면, 기존 타일을 그대로 가져옴
                newTiles[y][x] = tiles[y][x];
            }
            else
            {
                // 새로 확장된 영역이면, 새로운 타일을 생성
				const auto& tile = make_shared<Tile>();
                tile->SetPosition(Vector3((float)x * spacing, (float)y * spacing, 0.0f));
                tile->SetIndex(index);
                // 새로 생성되는 타일은 검은색으로 설정
                tile->SetColor(Values::Black);
                newTiles[y][x] = tile;
            }
            index++;
        }
    }

    // 기존 tiles를 새 tiles로 교체
    tiles = move(newTiles);
    // width/height를 갱신
    width = newWidth;
    height = newHeight;
}

void TileMap::Update()
{
	if (editNow == true)
	{
		// 예시: 카메라 위치를 고려하여 마우스 좌표를 월드 좌표로 변환
		Vector3 mPos = INPUT->GetMousePos();

		// 카메라가 직교 카메라이고, GetPosition()이 카메라의 월드 위치를 반환한다고 가정하면:
		mPos += CAMERA->GetPosition();

		Tile* tile = GetTile(mPos);

		if (tile) indexNumBuffer->SetIndex(tile->GetIndex());

		// 클릭 이벤트 처리 등 다른 입력 로직은 필요에 따라 여기에 추가
		// 예: 좌클릭 시 타일의 UV를 변경하는 등의 편집 작업
		if (INPUT->MousePressed(0x0000))
		{
			if (tile)
			{
				// 편집 작업은 별도로 처리(예: UV 변경 등)
				Vector2 startUV = TILESET->selectedStartUV;
				Vector2 endUV = startUV + TILESET->texelTileSize;
				tile->SetStartUV(startUV);
				tile->SetEndUV(endUV);
			}
		}
	}
	else
	{
		// 타일 위에 없으면 무효값(예: UINT_MAX)으로 설정
		indexNumBuffer->SetIndex(0xFFFFFFFF);
	}

}

void TileMap::Render()
{
	vertexBuffer->SetIA();
	indexBuffer->SetIA();
	inputLayout->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexShader->SetShader();
	pixelShader->SetShader();

	DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());

	// 타일셋 매니저에서 관리하는 SRV를 픽셀 셰이더에 바인딩
	DC->PSSetShaderResources(0, 1, TILESET->GetSRV().GetAddressOf());

	// 레스터라이저 및 블렌드 상태 설정
	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);


	// 각 타일마다 버텍스 버퍼를 업데이트하여 그리기
	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			shared_ptr<Tile> tile = tiles[y][x];
			if (!tile) continue;

			MapVertexBuffer();
			{
				// 각 타일에 맞게 버텍스 데이터 업데이트
				for (VertexTile& v : vertices)
				{
					v.color = tile->GetColor();
					v.index = tile->GetIndex();
				}

				vertices[0].uv = Vector2(tile->GetStartUV().x, tile->GetEndUV().y);
				vertices[1].uv = tile->GetStartUV();
				vertices[2].uv = tile->GetEndUV();
				vertices[3].uv = Vector2(tile->GetEndUV().x, tile->GetStartUV().y);

				// uv2 (추가 텍스처 좌표, 기본값)
				vertices[0].uv2 = Vector2(0, 1);
				vertices[1].uv2 = Vector2(0, 0);
				vertices[2].uv2 = Vector2(1, 1);
				vertices[3].uv2 = Vector2(1, 0);
			}
			UnmapVertexBuffer();

			world = Matrix::CreateTranslation(tile->GetPosition());
			worldBuffer->SetWorld(world);
			worldBuffer->SetVSBuffer(0);
			indexNumBuffer->SetPSBuffer(2);
			DC->DrawIndexed(indexBuffer->GetCount(), 0, 0);
		}
	}
}

void TileMap::ImGUI()
{
	// 타일셋 매니저의 GUI (타일 팔레트)
	TILESET->ImGUI();
}

void TileMap::Save(const string& path)
{
	ofstream fout(path);
	fout << to_string(width) << ' ' << to_string(height) << ' ' << to_string(spacing) << endl;

	// 타일셋 정보
	string imagePath = "";
	wstring tsPath = TILESET->GetTilePath();
	imagePath.assign(tsPath.begin(), tsPath.end());
	string xCount = to_string(TILESET->GetXCount());
	string yCount = to_string(TILESET->GetYCount());
	fout << imagePath << ' ' << xCount << ' ' << yCount << endl;

	// 각 타일의 UV 정보를 row-major 순서로 저장하면서,
	// 타일이 "빈 타일"이면 플래그 0, 아니면 1과 함께 UV정보 저장
	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			std::shared_ptr<Tile>& tile = tiles[y][x];
			// 예를 들어, 빈 타일은 색상이 기본 검은색(혹은 UV가 모두 0)으로 설정되어 있다고 가정
			bool isEmpty = (tile->GetStartUV() == Vector2(0, 0) && tile->GetEndUV() == Vector2(0, 0));
			fout << (isEmpty ? "0 " : "1 ");
			fout << to_string(tile->GetStartUV().x) << ' '
				<< to_string(tile->GetStartUV().y) << ' '
				<< to_string(tile->GetEndUV().x) << ' '
				<< to_string(tile->GetEndUV().y) << endl;
		}
	}
}

void TileMap::Load(const string& path)
{
	ifstream fin(path);
	if (fin.fail())
	{
		cout << "파일이 존재하지 않습니다. 이름을 정확히 적어주세요." << endl;
		return;
	}

	tiles.clear();
	char temp[256];

	// (1) width, height, spacing 읽기
	{
		fin.getline(temp, 256);
		istringstream iss(temp);
		iss >> width >> height >> spacing;
		if (width == 0 || height == 0 || spacing == 0) assert(false);
	}

	// (2) 타일셋 정보 읽기
	{
		fin.getline(temp, 256);
		istringstream iss(temp);
		string imagePath;
		UINT xCount = 0, yCount = 0;
		iss >> imagePath >> xCount >> yCount;
		wstring tsPath(imagePath.begin(), imagePath.end());
		TILESET->ChangeTileMap(tsPath, xCount, yCount);
	}

	// (3) 2차원 벡터 재생성 및 타일 생성
	tiles.resize(height);
	int index = 0;
	for (UINT y = 0; y < height; y++)
	{
		tiles[y].resize(width);
		for (UINT x = 0; x < width; x++)
		{
			const std::shared_ptr<Tile>& tile = make_shared<Tile>();
			// 위치 설정
			Vector3 position(static_cast<float>(x * spacing), static_cast<float>(y * spacing), 0.0f);
			tile->SetPosition(position);
			tile->SetIndex(index++);

			// UV 정보와 활성 플래그 읽기
			fin.getline(temp, 256);
			istringstream uvStream(temp);
			int activeFlag;
			float sx, sy, ex, ey;
			uvStream >> activeFlag >> sx >> sy >> ex >> ey;

			if (activeFlag == 1)
			{
				tile->SetStartUV(Vector2(sx, sy));
				tile->SetEndUV(Vector2(ex, ey));
				// 저장 시 빨간색이 임시로 설정되었더라도, 불러올 때는 기본 색상(예: 흰색)으로 덮어씁니다.
				tile->SetColor(Values::White);
			}
			else
			{
				// 빈 타일은 투명하게 처리
				tile->SetColor(Values::Transparent);
			}
			tiles[y][x] = tile;
		}
	}

	// 디버그용 백업 (원하는 경우)
	Save(BuildTilePath + "After.txt");
	cout << "타일맵 불러오기 완료!" << endl;
}

Tile* TileMap::GetTile(Vector3 mPos)
{
	UINT x = (int)(mPos.x) / spacing;
	UINT y = (int)(mPos.y) / spacing;
	if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
	return tiles[y][x].get();
}

