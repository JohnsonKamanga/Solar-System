#version 330 core
in vec2 TextCoord;

out vec4 FragColor;

uniform vec4 SphereColor;

void main(){
    FragColor = SphereColor; 
}