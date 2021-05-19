#version 330 core

uniform float screenWidth;
uniform float screenHeight;

uniform vec4 mainColor;
uniform vec4 lightColor;
uniform vec3 lightDirection;
uniform vec3 viewPosition;
out vec4 FragColor;

in vec3 normalVector;

vec3 fragCoord2NDC(vec3 fragCoord) {
    return vec3((fragCoord.x / screenWidth - 0.5) * 2,
        (fragCoord.y / screenHeight - 0.5) * 2,
        fragCoord.z - 0.5 * 2);
}

void main() {
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    float ambientStrength = 0.2;
    float diffuseStrength = 0.4;
    float specularStrength = 0.4;
    float specularShininess = 8;

    vec3 normalizedVector = normalize(normalVector);

    ambientColor = ambientStrength * vec4(1.0);
    diffuseColor = max(dot(lightDirection, normalizedVector), 0.0) * lightColor;

    vec3 viewDirection = normalize(fragCoord2NDC(gl_FragCoord.xyz) - viewPosition);
    vec3 reflectDirection = reflect(-lightDirection, normalizedVector);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), specularShininess);
    specularColor = specularStrength * specularValue * lightColor;

    FragColor = (ambientColor + diffuseColor + specularColor) * mainColor;
}
