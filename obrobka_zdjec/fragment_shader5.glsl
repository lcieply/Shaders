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
	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	//za przyciemnienie obrazu odpowiedzialny jest ostatni parametr
	pixelColors.rgba = vec4(texture(myTextureSampler, coord).rgb, value);

}
