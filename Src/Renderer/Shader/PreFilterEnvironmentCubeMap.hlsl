static const float PI = 3.14159265359;
static const float TwoPI = 2.0 * PI;

cbuffer PreFilterEnvironmentCubeMapConstants 
{
	float roughness;
};

struct CoordinateSystem
{
	float3 x;
	float3 y;
	float3 z;
};

TextureCube environmentCubeMap; 
RWTexture2DArray<float4> prefilterMap;

SamplerState environmentCubeMap_sampler;

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float RadicalInverseVdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float VanDerCorpus(uint n, uint base)
{
    float invBase = 1.0 / float(base);
    float denom   = 1.0;
    float result  = 0.0;
    for(uint i = 0u; i < 32u; ++i)
    {
        if(n > 0u)
        {
            denom   = fmod(float(n), 2.0);
            result += denom * invBase;
            invBase = invBase / 2.0;
            n       = uint(float(n) / 2.0);
        }
    }
    return result;
}

float2 Hammersley2D(uint i, unsigned int n)
{
	return float2((float)i / (float)n, RadicalInverseVdC(i));
	// return float2((float)i / (float)n, VanDerCorpus(i, 2u));
}

float3 importanceSampleGGX(float2 Xi, float roughness)
{
	float a = roughness * roughness;
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	float3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	return H;
}

float3 calculateSamplePicker(uint3 dispatchThreadId : SV_DispatchThreadID)
{
	float prefilterMapWidth, prefilterMapHeight, prefilterMapDepth;
	prefilterMap.GetDimensions(prefilterMapWidth, prefilterMapHeight, prefilterMapDepth);
    float2 uv = dispatchThreadId.xy/float2(prefilterMapWidth, prefilterMapHeight);
	uv.y = 1.0 - uv.y;
	uv = uv * 2.0 - 1.0;
	float3 SamplePicker = float3(1.0, 0.0, 0.0);
	switch(dispatchThreadId.z)
	{
	case 0: SamplePicker = float3(1.0,  uv.y, -uv.x); break;
	case 1: SamplePicker = float3(-1.0, uv.y,  uv.x); break;
	case 2: SamplePicker = float3(uv.x, 1.0, -uv.y); break;
	case 3: SamplePicker = float3(uv.x, -1.0, uv.y); break;
	case 4: SamplePicker = float3(uv.x, uv.y, 1.0); break;
	case 5: SamplePicker = float3(-uv.x, uv.y, -1.0); break;
	}
    return normalize(SamplePicker);
}

CoordinateSystem makeCoordinateSystem(const float3 N)
{
	const float3 UpVector = float3(0.0, 1.0, 0.0);
	CoordinateSystem system;
	system.x = normalize(cross(N, UpVector));
	system.y = normalize(cross(N, system.x));
	system.z = N;
	return system;
}

float3 convertCoordinateSystem(const float3 v, CoordinateSystem coordinateSystem)
{
	const float4x4 C = float4x4(float4(coordinateSystem.x, 0.0f), float4(coordinateSystem.y, 0.0f), float4(coordinateSystem.z, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
	const float4 v1 = mul(transpose(C), float4(v, 1.0f));
	return v1.xyz;
}

[numthreads(32, 32, 1)]
void main(uint3 groupId : SV_GroupID,
	uint3 groupThreadId : SV_GroupThreadID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint groupIndex : SV_GroupIndex)
{
	float3 N = calculateSamplePicker(dispatchThreadId);
	float3 V = N;
	CoordinateSystem coordinateSystem = makeCoordinateSystem(N);
	float3 prefilteredColor = 0.0;
	float totalWeight = 0.0;
	const uint sampleCount = 1024;
	for(uint i=0; i<sampleCount ; ++i) 
	{
		float2 Xi = Hammersley2D(i, sampleCount);
		float3 H = convertCoordinateSystem(importanceSampleGGX(Xi, roughness), coordinateSystem);
		float3 L = reflect(-H, V);
		float NdotL = dot(N, L);
		if(NdotL > 0.0)
		 {
			prefilteredColor  += environmentCubeMap.SampleLevel(environmentCubeMap_sampler, L, 0).xyz * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilteredColor /= totalWeight ;
	prefilterMap[dispatchThreadId] = float4(prefilteredColor, 1.0);
}
