cbuffer UmJunShik
{
	float4 colors[6];
};

// triangle id
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	// because there are 12 triangles but 6 faces (colors)
	return float4(colors[tid / 2].xyz, 1.f);
}