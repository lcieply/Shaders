//efekt przyciemnienia

#version 330 core 
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;

uniform float height;
uniform float width;

void main()
{	

	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec3 colors = texture(myTextureSampler, coord).rgb;
	float red = colors.r * 0.393 + colors.g * 0.769 + colors.b * 0.189;
	float green = colors.r * 0.349 + colors.g * 0.686 + colors.b * 0.168;
	float blue = colors.r * 0.272 + colors.g * 0.534 + colors.b * 0.131;
	pixelColors.rgba = vec4(red, green, blue, 1.0);

}
