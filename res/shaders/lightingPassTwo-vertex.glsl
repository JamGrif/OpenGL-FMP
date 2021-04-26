#version 430 
#define NUMBER_OF_POINT_LIGHTS 4
layout(location=0) in vec3 vertPos; //location=0 is used for the glVertexAttribPointer (first param) and glEnableVertexAttribArray(0)
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 vertTex;
layout(location=3) in vec3 vertTangent;
layout(location=4) in vec3 vertBitangent;


uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform vec3 viewPos;				


out vec3 varyingFragPos; //Fragment positions sent out from vertex shader
out vec2 varyingTexCoords;
out mat3 TBN;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pLight[NUMBER_OF_POINT_LIGHTS];


void main(void) 
{
	varyingFragPos = vec3(m_matrix * vec4(vertPos, 1.0));
	varyingTexCoords = vertTex;

	mat3 normalMatrix = transpose(inverse(mat3(m_matrix)));
	vec3 T = normalize(normalMatrix * vertTangent);
	vec3 N = normalize(normalMatrix * vertNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = transpose(mat3(T, B, N));
	TangentViewPos = TBN * viewPos;
	TangentFragPos = TBN * varyingFragPos; 

	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertPos,1.0); //Position of vertex in 3D space

}