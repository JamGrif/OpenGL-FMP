#version 430 
#define NUMBER_OF_POINT_LIGHTS 4


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	sampler2D normal;
	sampler2D height;
	float shininess;

	int normalizeTex;
	bool usingNormal;
	bool usingEmission;
	//bool usingDisplacement;
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

in vec3 varyingFragPos; //Fragment positions sent out from vertex shader
in vec2 varyingTexCoords;
in vec3 varyingNormal;
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


uniform vec3 viewPos;				//Camera position
uniform Material material;			//Surface material
uniform PointLight pLight[NUMBER_OF_POINT_LIGHTS];
uniform DirectionalLight dLight;
uniform SpotLight sLight;


vec3 calculateDirLight(DirectionalLight dl, vec3 normal);
vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 viewDir, vec2 ParallaxTexCoords);
vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 fragPos, vec3 viewDir);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main(void)
{
	vec3 norm;
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 ParallaxTexCoords = ParallaxMapping(varyingTexCoords, viewDir);

	if (material.usingNormal)
	{
		if (material.normalizeTex == 1)
		{
			norm = normalize(texture(material.normal, ParallaxTexCoords).rgb);
		}
		else if (material.normalizeTex == 0)
		{
			norm = texture(material.normal, varyingTexCoords).rgb;
		}
		norm = normalize(norm * 2.0 - 1.0);
	}
	else 
	{
		norm = normalize(varyingNormal);
	}	
	
	

	vec3 result;

	//Directional Light
	if (dLight.diffuse.x != 0.0) //Ensure a directional light exists by checking if it has a diffuse value
	{
		//result = calculateDirLight(dLight, norm);
	}
	
	//Point Light
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		if (pLight[i].diffuse.x != 0.0)
		{
			result += calculatePointLight(pLight[i], norm, viewDir, ParallaxTexCoords);
		}
		
	}

	//Spot Light
	if (sLight.diffuse.x != 0.0) //Ensure a spotlight exists by checking if it has a diffuse value
	{
		//result += calculateSpotLight(sLight, norm, varyingFragPos, viewDir);
	}

	//emission
	if (material.usingEmission)
	{
		vec3 emission = texture(material.emission, varyingTexCoords).rgb;
		result += emission;
	}
	

	fragColor = vec4(result, 1.0);

}

vec3 calculateDirLight(DirectionalLight dl, vec3 normal)
{
	vec3 lightDir;  // "-" as its from the surface, not from the light
	vec3 viewDir;

	if(material.usingNormal)
	{
		lightDir = normalize((TBN)*-dl.direction);
		viewDir = normalize(TangentViewPos - TangentFragPos);
	}
	else
	{
		lightDir = normalize(-dl.direction);
		viewDir = normalize(viewPos - varyingFragPos);
	}

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular 
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir),0.0),material.shininess);

	//Combine
	vec3 ambient = dl.ambient * vec3(texture(material.diffuse, varyingTexCoords));
	vec3 diffuse = dl.diffuse * diff * vec3(texture(material.diffuse, varyingTexCoords));
	vec3 specular = dl.specular * spec * vec3(texture(material.specular, varyingTexCoords));

	return (ambient + diffuse + specular);

}

vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 viewDir, vec2 ParallaxTexCoords)
{
	//Determine light space
	vec3 lightDir;
	//vec3 viewDir;
	float distance;

	if (material.usingNormal)
	{
		lightDir = normalize((TBN*pl.position) - TangentFragPos);
		//viewDir = normalize(TangentViewPos - TangentFragPos);
		distance = length((TBN*pl.position) - TangentFragPos);
	}
	else
	{
		lightDir = normalize(pl.position - varyingFragPos);
		//viewDir = normalize(viewPos - varyingFragPos);
		distance = length(pl.position - varyingFragPos);
	}

	//Diffuse
	float diff = max(dot(lightDir, normal), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	//Attenuation
	float attenuation = 1.0f / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));

	//Combine
	vec3 ambient = pl.ambient * texture(material.diffuse, ParallaxTexCoords).rgb;
	vec3 diffuse = pl.diffuse * diff * texture(material.diffuse, ParallaxTexCoords).rgb;
	vec3 specular = pl.specular * spec * texture(material.specular, ParallaxTexCoords).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);

}


vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(sl.position - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	//Attenuation
	float distance = length(sl.position - fragPos);
	float attenuation = 1.0f / (sl.constant + sl.linear * distance + sl.quadratic * (distance * distance));

	//Spotlight intensity
	float theta = dot(lightDir, normalize(-sl.direction));
	float epsilon = sl.cutOff - sl.outerCutOff;
	float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);

	//Combine
	vec3 ambient = sl.ambient * texture(material.diffuse, varyingTexCoords).rgb;
	vec3 diffuse = sl.diffuse * diff * texture(material.diffuse, varyingTexCoords).rgb;
	vec3 specular = sl.specular * spec * texture(material.specular, varyingTexCoords).rgb;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = texture(material.height, texCoords).r;
	vec2 p = viewDir.xy / viewDir.z * (height * 0.05);
	return texCoords - p;
}