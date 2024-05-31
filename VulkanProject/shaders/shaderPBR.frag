# version 450

layout(binding = 1) uniform sampler2D diffuseSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D roughnessSampler;
layout(binding = 4) uniform sampler2D metallicSampler;

layout(push_constant) uniform Constants {
	vec3 lightDirection;
	float lightIntensity;
	vec3 cameraPos;
	int useAlbedo;
    vec3 albedoValue;
	int useNormal;
    int useRoughness;
    float roughnessValue;
    int useMetal;
    float metalValue;
} constant;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec3 fragWorldPosition;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265358979323846;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

void main()
{
    vec3 albedo;
    float roughness;
    float metallic;
    vec3 normal;
    if(constant.useAlbedo == 1)
    {
        albedo = texture(diffuseSampler, fragTexCoord).rgb;
    }
    else
    {
        albedo = constant.albedoValue;
    }
    if(constant.useRoughness == 1)
	{
        roughness = texture(roughnessSampler, fragTexCoord).r;
	}
	else
	{
    	roughness = constant.roughnessValue;
	}
    if(constant.useMetal == 1)
    {
        metallic = texture(metallicSampler, fragTexCoord).r;
    }
    else
    {
        metallic = constant.metalValue;
    }
    if(constant.useNormal == 1)
    {
        normal = normalize(texture(normalSampler, fragTexCoord).rgb * 2.0 - 1.0);
    }
    else
    {
        normal = vec3(0.0, 0.0, 1.0);
    }

    vec3 ambient = vec3(0.03) * albedo;

    vec3 binormal = cross(fragNormal, fragTangent);
    mat3 tangentSpace = mat3(normalize(fragTangent), normalize(binormal), normalize(fragNormal));
    normal = normalize(tangentSpace * normal);

    vec3 viewDir = normalize(constant.cameraPos - fragWorldPosition);
    vec3 halfVector = normalize(viewDir + constant.lightDirection);

    vec3 tangentNormal = normalize(normal);

    float diff = max(dot(tangentNormal, constant.lightDirection), 0.0);

    // Fresnel-Schlick approximation
    vec3 F0;
    if(metallic == 0.0)
    {
        F0 = vec3(0.04);
    }
    else 
    {
        F0 = albedo;
    }
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - dot(viewDir, halfVector), 5.0);

    // Cook-Torrance BRDF
    float D = DistributionGGX(tangentNormal, halfVector, roughness);
    float G = GeometrySmith(tangentNormal, viewDir, constant.lightDirection, roughness);
    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(0.0, dot(tangentNormal, viewDir)) * max(0.0, dot(tangentNormal, constant.lightDirection)) + 0.001;
    vec3 specular = numerator / denominator;

    // final color
    vec3 kS = F;
    vec3 kD;
    if(metallic == 0.0)
    {
        kD = vec3(1.0 - F0.r, 1.0 - F0.g, 1.0 - F0.b);
    }
    else 
    {
        kD = vec3(0.0);
    }
    kD *= 1.0 - metallic;
    vec3 diffuse = (kD * albedo) / PI;



    outColor = vec4(diffuse + specular * constant.lightIntensity, 1.0);
}