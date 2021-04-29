#version 430 
out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform int screenFilter;

void main()
{
    if (screenFilter == 1)
    {
        color = texture(screenTexture, texCoords);
    }
    if (screenFilter == 2)
    {
        color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
    }
   
}