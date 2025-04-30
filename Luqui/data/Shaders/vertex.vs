#version 330 core

layout(location = 0) in vec3 aPos;       // Posición del vértice
layout(location = 1) in vec3 aNormal;    // Normal del vértice
layout(location = 2) in vec2 aTexCoords; // Coordenadas de textura

out vec2 TexCoords;
out vec3 FragPos;       // Posición del fragmento en el espacio mundial
out vec3 Normal;        // Normal del fragmento en el espacio mundial

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transformar la posición del vértice al espacio mundial
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transformar la normal al espacio mundial
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pasar las coordenadas de textura al fragment shader
    TexCoords = aTexCoords;

    // Transformar la posición del vértice al espacio de la cámara
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}