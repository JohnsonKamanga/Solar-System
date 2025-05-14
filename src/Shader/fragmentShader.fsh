#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D PlanetTexture;

//point light uniforms
uniform vec3 pointLightColor;
uniform vec3 pointLightPosition;
uniform float pointLightAmbientStrength;

//directional light uniforms
uniform vec3 directionalLightColor;
uniform vec3 directionalLightPosition;
uniform float directionalLightAmbientStrength;


uniform vec3 viewPos;

void main(){
    vec3 norm = normalize(Normal);

    //point light calculations
    vec3 pointLightDir = normalize(pointLightPosition - FragPos);

    float pointLightDiff = max(dot(norm, pointLightDir), 0.0);
    vec3 pointLightDiffuse = pointLightDiff * pointLightColor;

    vec3 pointLightAmbient = pointLightAmbientStrength * pointLightColor;

    float pointLightSpecularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 pointLightReflectDir = reflect(-pointLightDir, norm);
    float pointLightSpec = pow(max(dot(viewDir, pointLightReflectDir), 0.0), 32);
    vec3 pointLightSpecular = pointLightSpecularStrength * pointLightSpec * pointLightColor;

    vec4 pointLightResult = vec4(pointLightAmbient + (pointLightDiffuse + pointLightSpecular)/(1 + FragPos.z + FragPos.z * FragPos.z), 1.0f) * texture(PlanetTexture, TexCoord);

    //directional light calculations
    vec3 directionalLightDir = normalize(directionalLightPosition);

    float directionalLightDiff = max(dot(norm, directionalLightDir), 0.0);
    vec3 directionalLightDiffuse = directionalLightDiff * directionalLightColor;

    vec3 directionalLightAmbient = directionalLightAmbientStrength * directionalLightColor;

    float directionalLightSpecularStrength = 0.5;
    
    vec3 directionalLightReflectDir = reflect(-directionalLightDir, norm);
    float directionalLightSpec = pow(max(dot(viewDir, directionalLightReflectDir), 0.0), 32);
    vec3 directionalLightSpecular = directionalLightSpecularStrength * directionalLightSpec * directionalLightColor;

    vec4 directionalLightResult = vec4(directionalLightAmbient + directionalLightDiffuse + directionalLightSpecular, 1.0f) * texture(PlanetTexture, TexCoord);

    FragColor = pointLightResult + directionalLightResult;
}
