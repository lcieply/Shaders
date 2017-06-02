//efekt przyciemnienia

#version 330 core 
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;

uniform float height;
uniform float width;
uniform float value;
void main()
{	
	vec2 coord = vec2(gl_FragCoord.x/width, 

gl_FragCoord.y/height);
	vec4 texColor = texture(myTextureSampler, coord).rgba;
	float brightness = (0.2126*texColor.r) + 

(0.7152*texColor.g) + (0.0722*texColor.b);
	if (brightness > value) {
		coord.x = coord.x + sin(coord.y * 80) * 0.003;
		coord.y = coord.y + sin(coord.x * 80) * 0.003;
		vec3 colors = texture(myTextureSampler, 

coord).rgb;
		colors = floor(colors * 10) * 0.1;
		pixelColors.rgba = vec4(colors.rgb, 

1.0)*texColor;
	} else {
		pixelColors = floor(texColor * 10) * 0.1;
	}
}
