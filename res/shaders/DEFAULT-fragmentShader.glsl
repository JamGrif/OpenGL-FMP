#version 430 
//layout(binding=0) uniform sampler2D samp;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;



uniform vec3 viewPos;  //Camera position
uniform Material material; //Surface material
uniform PointLight pLight;





void main(void)
{
	//Ambient
	vec3 ambient = pLight.ambient * material.ambient;

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(pLight.position - fragPos); //Difference between tells the direction the light is pointing
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pLight.diffuse * (diff * material.shininess);

	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pLight.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	fragColor =  vec4(result, 1.0f);
	
}