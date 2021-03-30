#version 430 
//layout(binding=0) uniform sampler2D samp;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;



uniform vec3 viewPos;				//Camera position
uniform Material material;			//Surface material
uniform PointLight pLight;
uniform DirectionalLight dLight;





void main(void)
{
	//Ambient
	vec3 ambient = pLight.ambient * vec3(texture(material.diffuse, texCoords));
	//vec3 ambient = dLight.ambient * vec3(texture(material.diffuse, texCoords));

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(pLight.position - fragPos); //Difference between tells the direction the light is pointing
	//vec3 lightDir = normalize(-dLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = dLight.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = pLight.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pLight.specular * spec * vec3(texture(material.specular, texCoords));
	//vec3 specular = dLight.specular * spec * vec3(texture(material.specular, texCoords));

	//Attenuation
	float distance = length(pLight.position - fragPos);
	float attenuation = 1.0f / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	fragColor =  vec4(result, 1.0f);
	
}