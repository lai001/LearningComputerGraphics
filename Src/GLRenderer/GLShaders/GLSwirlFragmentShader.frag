#version 330 core

layout(location = 0) out vec4 color;
in vec2 out_textureCoordinate;

uniform sampler2D u_texture;

uniform vec2 center = vec2(0.5,0.5);
uniform float radius = 180.0;
uniform float angle = 90.0;

void main()
{
    vec2 textureCoordinateToUse = out_textureCoordinate;
    float dist = distance(center, out_textureCoordinate);
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

    color = texture2D(u_texture, textureCoordinateToUse);
};