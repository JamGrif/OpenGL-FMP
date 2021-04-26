#version 430 
#define NUMBER_OF_POINT_LIGHTS 4
layout(location=0) in vec3 vertPos; //location=0 is used for the glVertexAttribPointer (first param) and glEnableVertexAttribArray(0)
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 vertTex;
layout(location=3) in vec3 vertTangent;
layout(location=4) in vec3 vertBitangent;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	sampler2D normal;
	float shininess;

	int normalizeTex;
	bool usingNormal;
	bool usingEmission;
};
uniform Material material;			//Surface material

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform vec3 viewPos;				

out vec3 varyingFragPos; //Fragment positions sent out from vertex shader
out vec2 varyingTexCoords;
out vec3 varyingNormal;
out mat3 TBN;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main(void) 
{
	varyingFragPos = vec3(m_matrix * vec4(vertPos, 1.0));
	varyingTexCoords = vertTex;

	if (material.usingNormal)
	{
		mat3 normalMatrix = transpose(inverse(mat3(m_matrix)));
		vec3 T = normalize(normalMatrix * vertTangent);
		vec3 N = normalize(normalMatrix * vertNormal);
		T = normalize(T - dot(T, N) * N);
		vec3 B = cross(N, T);

		TBN = transpose(mat3(T, B, N));
		TangentViewPos = TBN * viewPos;
		TangentFragPos = TBN * varyingFragPos; 
	}
	else 
	{
		varyingNormal = mat3(transpose(inverse(m_matrix)))*vertNormal;
	}
	

	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertPos,1.0); //Position of vertex in 3D space

}