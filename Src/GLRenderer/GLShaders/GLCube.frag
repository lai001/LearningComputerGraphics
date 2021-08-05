#version 330 core

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	// 光衰减常量
	float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
	vec3 direction;
    float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	// 光衰减常量
	float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;
uniform int spotLightEnable;

float caculateAttenuationRatio(const float constant, const float linear, const float quadratic, float distance);
vec4 calculatePointLight();
vec4 calculateDirectionalLight();
vec4 calculateSpotLight();
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float LinearizeDepth(float depth);

void main()
{
	vec4 pointLight = calculatePointLight();
	vec4 directionalLight = calculateDirectionalLight();
	vec4 spotLight = calculateSpotLight();
	//textureColor
	vec4 textureColor = texture(material.diffuseTexture, TexCoord);


	if (bool(spotLightEnable))
	{
		FragColor = textureColor * (pointLight + directionalLight + spotLight);
	}
	else 
	{
		FragColor = textureColor * (pointLight + directionalLight);
	}

//	float depth = LinearizeDepth(gl_FragCoord.z) / 100.0;
//
//	 FragColor = vec4(vec3(depth), 1.0);
}



float LinearizeDepth(float depth) 
{
	float near = 0.1; 
	float far  = 100.0; 
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}


float caculateAttenuationRatio(const float constant, const float linear, const float quadratic, float distance)
{
	float attenuationRatio = 1.0 / (constant + linear * distance +  quadratic * (distance * distance));
	return attenuationRatio;
}

vec4 calculatePointLight()
{
	float lightToFragPosDistance = length(pointLight.position - FragPos);
	float attenuationRatio = caculateAttenuationRatio(pointLight.constant, pointLight.linear, pointLight.quadratic, lightToFragPosDistance);
	
    // ambient
    vec3 ambientColor = pointLight.ambient * attenuationRatio;
	
	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * vec3(texture(material.diffuseTexture, TexCoord)) * pointLight.diffuse * attenuationRatio;
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = spec * pointLight.specular * attenuationRatio * vec3(texture(material.specularTexture, TexCoord));
	vec4 mixLightingColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	return mixLightingColor;
}


vec4 calculateDirectionalLight()
{
    // ambient
    vec3 ambientColor = directionalLight.ambient;
	
	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * directionalLight.diffuse * vec3(texture(material.diffuseTexture, TexCoord));
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = spec * directionalLight.specular * vec3(texture(material.specularTexture, TexCoord));
	vec4 mixLightingColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	return mixLightingColor;
}

vec4 calculateSpotLight()
{
	float lightToFragPosDistance = length(spotLight.position - FragPos);
	float attenuationRatio = caculateAttenuationRatio(spotLight.constant, spotLight.linear, spotLight.quadratic, lightToFragPosDistance);

    // ambient
    vec3 ambientColor = spotLight.ambient * attenuationRatio;
	
	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * spotLight.diffuse * attenuationRatio * vec3(texture(material.diffuseTexture, TexCoord));
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = spec * spotLight.specular * attenuationRatio * vec3(texture(material.specularTexture, TexCoord));

	// spotlight intensity
    float theta = dot(lightDir, normalize(-spotLight.direction)); 
	float alpha		= (theta		   - spotLight.outerCutOff);
    float epsilon	= spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp(alpha / epsilon, 0.0, 1.0);

	// multiply spotlight intensity
	ambientColor *= intensity;
	diffuseColor *= intensity;
	specularColor *= intensity;

	vec4 mixLightingColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	return mixLightingColor;
}
