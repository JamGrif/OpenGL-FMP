#version 430 
#define NUMBER_OF_POINT_LIGHTS 4
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

	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
	
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
uniform PointLight pLight[NUMBER_OF_POINT_LIGHTS];
uniform DirectionalLight dLight;
uniform SpotLight sLight;

vec3 calculateDirLight(DirectionalLight dl, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 fragPos, vec3 viewDir);



void main(void)
{

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	//Directional Light
	vec3 result = calculateDirLight(dLight, norm, viewDir);

	//Point Light
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pLight[i], norm, fragPos, viewDir);
	}

	//Spot Light
	result += calculateSpotLight(sLight, norm, fragPos, viewDir);

	fragColor = vec4(result, 1.0);

	
}

vec3 calculateDirLight(DirectionalLight dl, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dl.direction); // "-" as its from the surface, not from the light

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular 
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);

	//Combine
	vec3 ambient = dl.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = dl.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = dl.specular * spec * vec3(texture(material.specular, texCoords));

	return (ambient + diffuse + specular);

}

vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(pl.position - fragPos);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Attenuation
	float distance = length(pl.position - fragPos);
	float attenuation = 1.0f / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));

	//Combine
	vec3 ambient = pl.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = pl.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = pl.specular * spec * vec3(texture(material.specular, texCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);

}


vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(sl.position - fragPos);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Attenuation
	float distance = length(sl.position - fragPos);
	float attenuation = 1.0f / (sl.constant + sl.linear * distance + sl.quadratic * (distance * distance));

	//Spotlight intensity
	float theta = dot(lightDir, normalize(-sl.direction));
	float epsilon = sl.cutOff - sl.outerCutOff;
	float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);

	//Combine
	vec3 ambient = sl.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = sl.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = sl.specular * spec * vec3(texture(material.specular, texCoords));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}