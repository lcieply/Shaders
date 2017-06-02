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
	vec3 texColors = texture(myTextureSampler, coord).rgb;
	float gray = (texColors.r + texColors.g + texColors.b)/3;
	pixelColors.rgba = vec4(vec3(gray).rgb, 1);

}
