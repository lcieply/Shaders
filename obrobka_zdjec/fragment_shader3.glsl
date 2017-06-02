//efekt rozmycia

#version 330 core 
//http://bazyluk.net/dydaktyka/AGKCR_2016/02_Teksturowanie_FBO_Bloom.pdf
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;

float w[5] = {0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};

uniform float height;
uniform float width;
uniform float value;
void main()
{
	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec3 colors = texture(myTextureSampler, coord).rgb * w[0];
	vec2 texelOffset = vec2(value*3);
	texelOffset = texelOffset / textureSize(myTextureSampler, 0);
	for (int i = 1; i < 5; ++i) {
		colors += texture(myTextureSampler, coord + texelOffset * i).rgb * w[i];
	}
	for(int i = 1 ; i < 5; i++){
		colors += texture(myTextureSampler, coord - texelOffset * i).rgb * w[i];
	}
	pixelColors = vec4(colors.rgb, 1.0);
}
