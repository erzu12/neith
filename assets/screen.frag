#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

const float offset = 1.0 / 600.0;

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


void main() {

	//vec2 offsets[9] = vec2[](
	//	vec2(-offset,  offset), // top-left
	//	vec2( 0.0f,    offset), // top-center
	//	vec2( offset,  offset), // top-right
	//	vec2(-offset,  0.0f),   // center-left
	//	vec2( 0.0f,    0.0f),   // center-center
	//	vec2( offset,  0.0f),   // center-right
	//	vec2(-offset, -offset), // bottom-left
	//	vec2( 0.0f,   -offset), // bottom-center
	//	vec2( offset, -offset)  // bottom-right    
	//);
	//
	//float kernel[9] = float[](
	//	2, 2, 2,
	//	2, -16, 2,
	//	2, 2, 2
	//);
	//
	//vec3 sampleTex[9];
	//for(int i = 0; i < 9; i++) {
	//	sampleTex[i] = texture(screenTexture, TexCoords.st + offsets[i]).rgb;
	//}
	//vec3 col = vec3(0.0);
	//for(int i = 0; i < 9; i++)
	//	col += sampleTex[i] * kernel[i];

    FragColor = texture(screenTexture, TexCoords);
    float depth = texture(depthTexture, TexCoords).r;
    FragColor.rgb =  aces(FragColor.rgb);

    float fogAmount = -0.0002/(depth - 1.00001);
    fogAmount = clamp(fogAmount, 0.0, 1.0);
    //vec3 color = FragColor.rgb * 0.0001 + vec3(fogAmount);
    FragColor.rgb = mix(FragColor.rgb, vec3(0.3, 0.8, 1.0), fogAmount * 0.3);

	float gamma = 2.2;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));	
    //FragColor = vec4(color, 1.0);

}
