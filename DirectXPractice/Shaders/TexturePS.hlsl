// the explicit way of specifying the slot is
// Texture2D tex : register(t0);
Texture2D tex;
SamplerState samp;

float4 main(float2 texCoord : TexCoord) : SV_Target
{
	// kind of like a getter. Gets a pixel value 
	// from this texture using this sampler.
	return tex.Sample(samp, texCoord);
}