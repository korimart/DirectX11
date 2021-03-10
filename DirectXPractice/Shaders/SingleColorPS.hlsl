cbuffer Light
{
	float3 lightViewPos;
};

cbuffer Material
{
	float3 materialColor;
};

float4 main() : SV_Target
{
	return float4(materialColor, 1.f);
}
