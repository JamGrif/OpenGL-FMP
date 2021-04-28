#version 430 
out vec4 color;

in vec3 texCoords;

struct Sky
{
    samplerCube skybox;
};

uniform Sky sky;


void main()
{
    color = texture(sky.skybox, texCoords);
}