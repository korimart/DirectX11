cbuffer Whatever
{
	matrix transform;
};

struct VSOut
{
	float2 tex : TexCoord; // tex first so PS parameter can be just this one only.
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
	VSOut vsout;
	vsout.tex = tex;
	vsout.pos = mul(float4(pos, 1.f), transform);
	return vsout;
}
