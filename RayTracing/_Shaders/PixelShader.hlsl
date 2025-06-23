Texture2D baseColorTexture : register(t0); // 텍스쳐는 t를 붙이고 0~255 까지 들어감
SamplerState baseColorSampler : register(s0); // 샘플러는 s0 이런식으로 등록함.

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
    return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}