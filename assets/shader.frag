#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D roughness;
    sampler2D normal;
    sampler2D specular;
    sampler2D metallic;
    sampler2D alpha;
};

struct Light {
    vec3 direction;
    vec3 color;
};


in VS_OUT {
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D shadowMap;
//uniform samplerCube cubeMap;

const float PI = 3.14159265359;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.002);
    float shadow = 0.0;
    vec2 textlSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * 0.5 * textlSize).r;
            shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 1.0;

    return shadow;
}

vec3 fresnelSchlick(float cosTheta, vec3 FO) {
    return FO + (1.0 - FO) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{
    vec4 diffuseAlpha = texture(material.diffuse, fs_in.TexCoord).rgba;
    //float alpha = diffuseAlpha.a;
    float alpha = texture(material.alpha, fs_in.TexCoord).r;
    if(alpha < 0.7)
        discard;

    vec3 diffuse = diffuseAlpha.rgb;
    float roughness = texture(material.roughness, fs_in.TexCoord).r;
    vec3 N = texture(material.normal, fs_in.TexCoord).rgb;
    float specular = texture(material.specular, fs_in.TexCoord).r;
    float metallic = texture(material.metallic, fs_in.TexCoord).r;

    N = normalize(N * 2.0 - 1.0);
    N.z += 3.0;
    N = normalize(N);

    vec3 V = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 Lo = vec3(0.0);
    //Sun Light
    vec3 L = normalize(-fs_in.TangentLightDir);
    vec3 H = normalize(V + L);
    vec3 radiance = light.color;

    vec3 FO = vec3(specular * 0.08);
    FO = mix(FO, diffuse, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), FO);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 spec = numerator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * diffuse / PI + spec) * radiance * NdotL;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, N, L);

    vec3 ambient = vec3(0.2, 0.22, 0.26) * diffuse;
    vec3 color = ambient + Lo * shadow;

    //vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    //projCoords = projCoords * 0.5 + 0.5;
    //FragColor = vec4(texture(shadowMap, fs_in.TexCoord).r, 0.0, 0.0 , 1.0);
    //FragColor = vec4(fs_in.TexCoord, 0.0 , 1.0);
    FragColor = vec4(color * alpha, 1.0);
}
