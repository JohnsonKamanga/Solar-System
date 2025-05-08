#version 330 core

in vec3 Normals;
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D PlanetTexture;

void main(){
    FragColor = texture(PlanetTexture, TextCoord);
}
