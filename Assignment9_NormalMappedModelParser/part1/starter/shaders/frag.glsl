// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates.
in vec3 FragPos;
in vec2 v_texCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

void main()
{
    vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Convert from [0,1] to [-1,1]

    // Apply TBN matrix to normals
    vec3 fragNormal = normal;

    // Get direction to light source
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    // Calculate the diffuse impact by dot product of fragment normal and light direction
    float diff = max(dot(fragNormal, lightDir), 0.0);

    // Calculate the specular impact
    vec3 viewDir = normalize(-TangentFragPos); // View direction in tangent space
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Hard-coded shininess

    vec3 textureColor = texture(u_DiffuseMap, v_texCoord).rgb;
    vec3 ambient = 0.1 * textureColor; // Ambient color (hard-coded)
    vec3 diffuse = diff * textureColor; // Diffuse color
    vec3 specular = vec3(0.5) * spec; // Specular color (hard-coded)

    // Combine diffuse and specular components
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
// ==================================================================