cbuffer Transform
{
	matrix worldViewProj;
	matrix worldView;
};

struct VSOut
{
	float3 viewPos : Position;
	float3 viewNormal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 normal : Normal)
{
	VSOut vsOut;
	vsOut.viewPos = mul(float4(pos, 1.f), worldView);
	vsOut.viewNormal = mul(normal, (float3x3)worldView);
	vsOut.pos = mul(float4(pos, 1.f), worldViewProj);
	
	return vsOut;
}