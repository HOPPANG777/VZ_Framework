#pragma once

class WorldBuffer : public ConstantBuffer
{
	// 16바이트 단위로 데이터 패딩을 해줘야 한다.
	struct WorldStruct
	{
		Matrix world;
	};

	WorldStruct data;

public:
	WorldBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetWorld(const Matrix& world)
	{
		data.world = XMMatrixTranspose(world);
	}
};

class ViewProjectionBuffer : public ConstantBuffer
{
	struct ViewProjStruct
	{
		Matrix view;
		Matrix proj;
	};

	ViewProjStruct data;

public:
	ViewProjectionBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetView(const Matrix& view)
	{
		data.view = XMMatrixTranspose(view);
	}

	void SetProjection(const Matrix& proj)
	{
		data.proj = XMMatrixTranspose(proj);
	}
};

