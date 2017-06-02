//efekt przyciemnienia

#version 330 core 
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;

uniform vec2 posCursor;
uniform float height;
uniform float width;
uniform float value;
void main()
{	

	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec2 trans = 0.005 *( gl_FragCoord.xy - posCursor);
	
	float d = length(gl_FragCoord.xy)/100.0f;
	trans *=d;
	trans *= value;
	float border = (0.07)*log(length(trans));
	vec2 mcen = -border*normalize(trans);
	pixelColors.rgba = vec4(texture(myTextureSampler, coord-mcen).rgb, 1.0);

}
