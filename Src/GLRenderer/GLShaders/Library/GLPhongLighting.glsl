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
	int isEnable;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
	int isEnable;
};

struct SpotLight {
    vec3 position;
	vec3 direction;
    float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
	int isEnable;
};

float phongAttenuationRatio(const float constant, const float linear, const float quadratic, float distance)
{
	return 1.0 / (constant + linear * distance +  quadratic * (distance * distance));
}

vec4 phongPointLight(PointLight iPointLight,
					Material iMaterial,
					vec2 iUV,
					vec3 iNormal,
					vec3 iViewPosition,
					vec3 iFragPos)
{
	float lightToFragPosDistance = length(iPointLight.position - iFragPos);
	float attenuationRatio = phongAttenuationRatio(iPointLight.constant, iPointLight.linear, iPointLight.quadratic, lightToFragPosDistance);
	
    // ambient
    vec3 ambientColor = iPointLight.ambient * attenuationRatio;
	
	// diffuse 
    vec3 norm = normalize(iNormal);
    vec3 lightDir = normalize(iPointLight.position - iFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 lightDiffuseColor = diff * vec3(texture(iMaterial.diffuseTexture, iUV)) * iPointLight.diffuse * attenuationRatio;
	
	// specular
	vec3 viewDir = normalize(iViewPosition - iFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), iMaterial.shininess);
	vec3 lightSpecularColor = spec * iPointLight.specular * attenuationRatio * vec3(texture(iMaterial.specularTexture, iUV));

	vec4 mixLightingColor = vec4(ambientColor + lightDiffuseColor + lightSpecularColor, 1.0);
	return mixLightingColor * float(clamp(iPointLight.isEnable, 0, 1));
}


vec4 phongDirectionalLight(DirectionalLight iDirectionalLight,
							Material iMaterial,
							vec2 iUV,
							vec3 iNormal,
							vec3 iViewPosition,
							vec3 iFragPos)
{
    // ambient
    vec3 ambientColor = iDirectionalLight.ambient;
	
	// diffuse 
    vec3 norm = normalize(iNormal);
    vec3 lightDir = normalize(-iDirectionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * iDirectionalLight.diffuse * vec3(texture(iMaterial.diffuseTexture, iUV));
	
	// specular
	vec3 viewDir = normalize(iViewPosition - iFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), iMaterial.shininess);
	vec3 specularColor = spec * iDirectionalLight.specular * vec3(texture(iMaterial.specularTexture, iUV));

	vec4 mixLightingColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	return mixLightingColor * float(clamp(iDirectionalLight.isEnable, 0, 1));
}

vec4 phongSpotLight(SpotLight iSpotLight,
					Material iMaterial,
					vec2 iUV,
					vec3 iNormal,
					vec3 iViewPosition,
					vec3 iFragPos)
{
	float lightToFragPosDistance = length(iSpotLight.position - iFragPos);
	float attenuationRatio = phongAttenuationRatio(iSpotLight.constant, iSpotLight.linear, iSpotLight.quadratic, lightToFragPosDistance);

    // ambient
    vec3 ambientColor = iSpotLight.ambient * attenuationRatio;
	
	// diffuse 
    vec3 norm = normalize(iNormal);
    vec3 lightDir = normalize(iSpotLight.position - iFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * iSpotLight.diffuse * attenuationRatio * vec3(texture(iMaterial.diffuseTexture, iUV));
	
	// specular
	vec3 viewDir = normalize(iViewPosition - iFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), iMaterial.shininess);
	vec3 specularColor = spec * iSpotLight.specular * attenuationRatio * vec3(texture(iMaterial.specularTexture, iUV));

	// spotlight intensity
    float theta = dot(lightDir, normalize(-iSpotLight.direction)); 
	float alpha		= (theta		   - iSpotLight.outerCutOff);
    float epsilon	= iSpotLight.cutOff - iSpotLight.outerCutOff;
    float intensity = clamp(alpha / epsilon, 0.0, 1.0);

	// multiply spotlight intensity
	ambientColor *= intensity;
	diffuseColor *= intensity;
	specularColor *= intensity;

	vec4 mixLightingColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	return mixLightingColor * float(clamp(iSpotLight.isEnable, 0, 1));
}

vec4 phongCompositionLightColor(DirectionalLight iDirectionalLight,
								PointLight iPointLight,
								SpotLight iSpotLight,
								Material iMaterial,
								vec2 iUV,
								vec3 iNormal,
								vec3 iViewPosition,
								vec3 iFragPos)
{
	vec4 pointLight = phongPointLight(iPointLight,
									iMaterial,
									iUV,
									iNormal,
									iViewPosition,
									iFragPos);
	vec4 directionalLight = phongDirectionalLight(iDirectionalLight,
												iMaterial,
												iUV,
												iNormal,
												iViewPosition,
												iFragPos);
	vec4 spotLight = phongSpotLight(iSpotLight,
									iMaterial,
									iUV,
									iNormal,
									iViewPosition,
									iFragPos);
	return (pointLight + directionalLight + spotLight);
}