struct VSInput
{
	float3 Position		: ATTRIB0;
};

cbuffer SkyBoxVSHConstants
{
	float4x4 View;
	float4x4 Projection;
};

struct PSInput
{
	float4 Position		: SV_POSITION;
	float3 TexCoords	: TEXCOORDS;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
	PSIn.Position = mul(Projection, mul(View, float4(VSIn.Position, 1.0f))).xyww;
    PSIn.TexCoords = VSIn.Position;
}
