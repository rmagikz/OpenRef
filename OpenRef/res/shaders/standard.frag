#version 330 core

in vec2 v_TexCoord;
in float v_TexIndex;

in vec4 pp;

out vec4 color;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	color = texture(u_Textures[index], v_TexCoord);
};