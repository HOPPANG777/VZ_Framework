// GouraudShading.hlsl

struct VS_INPUT
{
    float3 pos : POSITION; // 정점 위치
    float3 normal : NORMAL; // 정점 법선
};

struct VS_OUTPUT
{
    float4 posH : SV_POSITION; // 변환된 위치
    float4 color : COLOR; // 정점에서 계산한 색상
};

cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer LightBuffer : register(b1)
{
    float3 LightDir;
    float3 LightColor;
    float3 AmbientColor;
    float3 ObjectColor;
}

// 정점 셰이더: 정점 단위로 조명 계산
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 월드 변환
    float4 worldPos = mul(float4(input.pos, 1.0), World);
    output.posH = mul(mul(worldPos, View), Projection);

    // 월드 공간에서의 법선
    float3 N = normalize(mul(input.normal, (float3x3) World));
    float3 L = normalize(-LightDir);

    // Lambert (디퓨즈)
    float diff = saturate(dot(N, L));

    float3 ambient = AmbientColor * ObjectColor;
    float3 diffuse = LightColor * ObjectColor * diff;

    output.color = float4(ambient + diffuse, 1.0);

    return output;
}

// 픽셀 셰이더: 보간된 색상 그대로 출력
float4 PS_Main(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}