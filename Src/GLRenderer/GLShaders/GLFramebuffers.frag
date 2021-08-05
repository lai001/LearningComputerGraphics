#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform vec2 center = vec2(0.5,0.5);
uniform float radius = 180.0;
uniform float angle = 90.0;

uniform int postProcessing;

vec4 normal();
vec4 reversedPhase();
vec4 gray();
vec4 kernel();
vec4 blur();
vec4 swirl();

void main()
{
	if (postProcessing == 0)
	{
		FragColor = normal();
	}
	else if (postProcessing == 1)
	{
		FragColor = reversedPhase();
	}
	else if (postProcessing == 2)
	{
		FragColor = gray();
	}
	else if (postProcessing == 3)
	{
		FragColor = kernel();
	}
	else if (postProcessing == 4)
	{
		FragColor = blur();
	}
	else if (postProcessing == 5)
	{
		FragColor = swirl();
	}
	else 
	{
		FragColor = normal();
	}
} 

vec4 normal()
{
	vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
	return FragColor;
}

vec4 reversedPhase()
{
	FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	return FragColor;
}

vec4 gray()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
	return FragColor;
}

vec4 kernel()
{
	const float offset = 1.0 / 300.0;  

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),  
        vec2( 0.0f,    0.0f),  
        vec2( offset,  0.0f),  
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset) 
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
	return FragColor;
}

vec4 blur()
{
	const float offset = 1.0 / 300.0;  

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),  
        vec2( 0.0f,    0.0f),  
        vec2( offset,  0.0f),  
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset) 
    );

	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
	return FragColor;
}

vec4 swirl()
{
	vec2 textureCoordinateToUse = TexCoords;
    float dist = distance(center, TexCoords);
    textureCoordinateToUse -= center;
    if (dist < radius)
    {
         float percent = (radius - dist) / radius;
         float theta = percent * percent * angle * 8.0;
         float s = sin(theta);
         float c = cos(theta);
        textureCoordinateToUse = vec2(dot(textureCoordinateToUse, vec2(c, -s)), dot(textureCoordinateToUse, vec2(s, c)));
    }
    textureCoordinateToUse += center;

    FragColor = texture2D(screenTexture, textureCoordinateToUse);
	return FragColor;
}