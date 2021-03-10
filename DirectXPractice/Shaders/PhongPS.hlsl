cbuffer Light
{
	float3 lightViewPos;
};

cbuffer Material
{
	float3 materialColor;
};

float4 main(
	float3 viewPos : Position, 
	float3 viewNormal : Normal) : SV_Target
{
	float3 lightDir = normalize(lightViewPos - viewPos);
	float lightAmountScale = max(0.0f, dot(lightDir, viewNormal));
	float3 diffuse = materialColor * lightAmountScale;
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	return float4(diffuse + ambient, 1.f);
}