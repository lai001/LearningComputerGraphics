cbuffer PBRVSHConstants
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
};

struct VSInput
{
	float3 Position		: ATTRIB0;
	float3 Normal		: ATTRIB1;
	float3 Tangent		: ATTRIB2;
	float3 Bitangent	: ATTRIB3;
	float2 TexCoords	: ATTRIB4;
};

struct PSInput
{
	float4 Position		: SV_POSITION;
	float4 FragPosition	: FRAGPOSITION;
	float3 Normal		: NORMAL;
	float2 TexCoords	: TEXCOORDS;
	float3x3 TBN		: TBN0;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
	float4x4 MV = mul(View, Model);
	float4x4 MVP = mul(Projection, MV);
	PSIn.Position = mul(MVP, float4(VSIn.Position, 1.0));
	PSIn.FragPosition = mul(Model, float4(VSIn.Position, 1.0));
	PSIn.Normal = VSIn.Normal;
	PSIn.TexCoords = VSIn.TexCoords;
	PSIn.TBN = float3x3(
		normalize(mul(Model, float4(VSIn.Tangent, 0.0f)).xyz), 
		normalize(mul(Model, float4(VSIn.Bitangent, 0.0f)).xyz),
		normalize(mul(Model, float4(VSIn.Normal, 0.0f)).xyz)
	);
	PSIn.TBN = transpose(PSIn.TBN);
}
