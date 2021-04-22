#version 430 
layout(location=0) in vec3 vertPos; //location=0 is used for the glVertexAttribPointer (first param) and glEnableVertexAttribArray(0)
layout(location=1) in vec2 vertTex;
layout(location=2) in vec3 vertNormal;



uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec3 varyingFragPos; //Fragment positions sent out from vertex shader
out vec2 varyingTexCoords;
out vec3 varyingNormal;	


void main(void) 
{
	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertPos,1.0); //Position of vertex in 3D space

	varyingFragPos = vec3(m_matrix * vec4(vertPos, 1.0f));
	varyingTexCoords = vertTex;
	varyingNormal = mat3(transpose(inverse(m_matrix))) * vertNormal;

}