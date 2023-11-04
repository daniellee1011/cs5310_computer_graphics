#version 410 core

in vec3 v_vertexNormals;
in vec2 v_texCoords; // Receive texture coordinates
in vec3 FragPos;

out vec4 color;

uniform vec3 u_CameraPosition;
uniform sampler2D u_diffuseMap; // Texture sampler for the diffuse map
uniform sampler2D u_bumpMap;    // Texture sampler for the bump map
uniform sampler2D u_specularMap;// Texture sampler for the specular map

void main()
{
    vec3 norm = normalize(v_vertexNormals);

    // Texture sampling
    vec3 sampledDiffuse = texture(u_diffuseMap, v_texCoords).rgb;
    vec3 sampledSpecular = texture(u_specularMap, v_texCoords).rgb;

    vec3 viewDir = normalize(u_CameraPosition - FragPos);

    // vec3 result = sampledDiffuse + sampledSpecular;
    vec3 result = sampledDiffuse;

    color = vec4(result, 1.0f);
    // color = texture(u_diffuseMap, v_texCoords);
    // color = vec4(v_texCoords, 0.0, 1.0); // UVs as red-green color
    // color = vec4(1.0, 0.0, 0.0, 1.0); // Red color

    // color = vec4(sampledDiffuse, 1.0);
}
