struct VSInput
{
	float2 Position	    : ATTRIB0;
	float2 TexCoords    : ATTRIB1;
};

struct PSInput
{
	float4 Position		: SV_POSITION;
	float2 TexCoords	: TEXCOORDS;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
	PSIn.Position = float4(VSIn.Position, 0.0, 1.0);
    PSIn.TexCoords = VSIn.TexCoords;
}
