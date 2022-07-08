struct VSInput
{
	float3 Position		: ATTRIB0;
	float3 Normal		: ATTRIB1;
	float2 TexCoords	: ATTRIB2;
};

struct PSInput
{
	float4 Position	: SV_POSITION;
};

cbuffer DirectionLightDepthMapVSHConstants
{
    float4x4 LightSpaceMat;
    float4x4 Model;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(LightSpaceMat, mul(Model, float4(VSIn.Position, 1.0f)));
}