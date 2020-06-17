#version 330

in vec2 TexCoord;

out vec4 color;

uniform sampler2D mTexture;

void main()
{
    color = texture(mTexture, TexCoord);
}