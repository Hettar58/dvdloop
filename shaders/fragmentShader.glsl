#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D tex;
uniform vec3 col;
void main()
{
	FragColor = texture(tex, TexCoord) * vec4(col.r, col.g, col.b, 1.0f);
}