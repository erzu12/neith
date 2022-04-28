#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 aModel;

out VS_OUT {
	vec2 TexCoord;
	vec3 FragPos;
	vec3 Normal;
	vec3 TangentLightDir;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec4 FragPosLightSpace;
} vs_out;

struct Light {
	vec3 direction;
	vec3 color;
};

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(aModel * vec4(aPos, 1.0));
	vs_out.Normal = mat3(transpose(inverse(aModel))) * aNormal;  //!This is inefficient to do in shaders!!!
	vs_out.TexCoord = aTexCoord;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	gl_Position = projection * view * aModel * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	vec3 T = normalize(vec3(aModel * vec4(aTangent.xyz, 0.0)));
	vec3 N = normalize(vec3(aModel * vec4(aNormal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TangentLightDir = TBN * light.direction;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentFragPos = TBN * vec3(aModel * vec4(aPos, 1.0));
}
