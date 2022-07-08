cbuffer PhongShadingVSHConstants
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
	float4x4 LightSpaceMatrix;
};

struct VSInput
{
	float3 Position		: ATTRIB0;
	float3 Normal		: ATTRIB1;
	float2 TexCoords	: ATTRIB2;
};

struct PSInput
{
	float4 Position			 : SV_POSITION;
	float4 FragPosition		 : FRAGPOSITION;
	float3 Normal			 : NORMAL;
	float2 TexCoords		 : TEXCOORDS;
	float4 FragPosLightSpace : FRAGPOSLIGHTSPACE;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
	float4x4 MV = mul(View, Model);
	float4x4 MVP = mul(Projection, MV);
	PSIn.Position = mul(MVP, float4(VSIn.Position, 1.0));
	PSIn.FragPosition = mul(Model, float4(VSIn.Position, 1.0));
	PSIn.Normal = VSIn.Normal;
	PSIn.TexCoords = VSIn.TexCoords;
	PSIn.FragPosLightSpace = mul(LightSpaceMatrix, float4(PSIn.FragPosition.xyz, 1.0));
}
