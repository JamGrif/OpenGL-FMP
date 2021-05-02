#version 430 

uniform mat4 mvp_matrix;

layout (vertices = 1) out; //Number of vertices "per patch" being passed from the vertex shader to control shader

//Control shader controls the topology of the triangle mesh that will be produced
//Executes once per incoming vertex
void main()
{
    gl_TessLevelOuter[0] = 6;
    gl_TessLevelOuter[1] = 6;
    gl_TessLevelOuter[2] = 6;
    gl_TessLevelOuter[3] = 6;
    gl_TessLevelInner[0] = 12;
    gl_TessLevelInner[1] = 12;
}