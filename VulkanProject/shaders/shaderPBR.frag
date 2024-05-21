# version 450

layout(binding = 1) uniform sampler2D diffuseSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D roughnessSampler;
layout(binding = 4) uniform sampler2D glossSampler;

layout(push_constant) uniform Constants {
	vec3 lightDirection;
	float lightIntensity;
	vec3 cameraPos;
	float shininess;
	bool useNormal;
} constant;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec3 fragWorldPosition;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265358979323846;


vec3 LambertDiffuse(float kd, vec3 cd)
{
	return (kd*cd) / PI;
}

float NormalDistribution(float roughness, float cosTheta)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotH = cosTheta;
    float NdotH2 = NdotH * NdotH;
    float num = alpha2;
    float denom = (NdotH2 * (alpha2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

float GGX(float roughness, float cosTheta, float NDF, vec3 viewDirection, vec3 normal)
{
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float NdotV = max(dot(normal, viewDirection), 0.0);
    float NdotL = max(dot(normal, constant.lightDirection), 0.0);
    float G = (NdotL * (1.0 - k) + k) * (NdotV * (1.0 - k) + k);
    return NdotL * NdotV / G;
}

vec3 BRDF(float NDF, float G, vec3 F, vec3 color, vec3 normal, vec3 viewDirection)
{
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDirection), 0.0) * max(dot(normal, normalize(constant.lightDirection)), 0.0) + 0.001;
    return numerator / denominator;
}

vec3 FresnelSchlick(vec3 F0, vec3 viewDirection, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 albedo = texture(diffuseSampler, fragTexCoord).rgb;
    float roughness = texture(roughnessSampler, fragTexCoord).r;
    vec3 normal = normalize(texture(normalSampler, fragTexCoord).rgb * 2.0 - 1.0);
    
    vec3 viewDirection = normalize(constant.cameraPos - fragWorldPosition);
    vec3 F0 = vec3(0.04);
    float cosTheta = max(dot(normal, viewDirection), 0.0);
    vec3 F = FresnelSchlick(F0, viewDirection, cosTheta);
    float NDF = NormalDistribution(roughness, cosTheta);
    float G = GGX(roughness, cosTheta, NDF, viewDirection, normal);
    vec3 specular = BRDF(NDF, G, F, albedo, normal, viewDirection);

    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - max(roughness, 0.04);

    vec3 radiance = (kD * albedo / PI + specular) * max(dot(normal, constant.lightDirection), 0.0);

    outColor = vec4(radiance, 1.0);
}