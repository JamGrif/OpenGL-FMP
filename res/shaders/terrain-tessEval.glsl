#version 430 

uniform mat4 mvp_matrix;
layout (quads, equal_spacing, ccw) in; //Unrelated to "out" in control shader, Instruct tessellator to generate vertices arranged in a quad, ccw specifies widning order of vertices and equal spacing of sub divisions

//Evaluation shader executes once per vertex produced by tesselator
void main()
{
   float u = gl_TessCoord.x;
   float v = gl_TessCoord.y;
   gl_Position = mvp_matrix * vec4(u,0,v,1);
}