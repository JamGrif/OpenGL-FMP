#version 430 
out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform int screenFilter;

void main()
{
    if (screenFilter == 1) //Default
    {
        color = texture(screenTexture, texCoords);
    }
    if (screenFilter == 2) //Inverse
    {
        color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
    }
    if (screenFilter == 3) //Greyscale
    {
        color = texture(screenTexture, texCoords);
        float average = (color.r + color.g + color.b) / 3.0;
        color = vec4(average, average, average, 1.0);
    }
    if (screenFilter == 4) //Blur
    {
    
    }
}