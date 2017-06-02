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
uniform float red;
uniform float green;
uniform float blue;
void main()
{	
	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec3 colors = texture(myTextureSampler, coord).rgb;
	colors.r *= red;
	colors.g *= green;
	colors.b *= blue;
	pixelColors.rgba = vec4(colors.rgb, 2*value);

}
