#version 430 
out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    color = texture(screenTexture, texCoords);
}