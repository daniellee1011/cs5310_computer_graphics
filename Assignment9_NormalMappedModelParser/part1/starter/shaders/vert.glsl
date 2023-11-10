// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0) in vec3 position;
layout(location=1) in vec3 normals;
layout(location=2) in vec2 texCoord;
layout(location=3) in vec3 tangents;
layout(location=4) in vec3 bitangents;

// If we have texture coordinates we can now use this as well
out vec3 FragPos;
out vec2 v_texCoord;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

// If we are applying our camera, then we need to add some uniforms.
// Note that the syntax nicely matches glm's mat4!

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projectionMatrix * modelTransformMatrix * vec4(position, 1.0f);

    vec4 FragPos4 = modelTransformMatrix * vec4(position, 1.0);
    FragPos = FragPos4.xyz;

    v_texCoord = texCoord;

    // Compute the TBN matrix
    mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix)));
    vec3 T = normalize(normalMatrix * tangents);
    vec3 B = normalize(normalMatrix * bitangents);
    vec3 N = normalize(normalMatrix * normals);
    mat3 TBN = mat3(T, B, N);

    // Transform the light and view position to tangent space
    TangentLightPos = TBN * (lightPos - FragPos);
    TangentViewPos = TBN * (viewPos - FragPos);
    TangentFragPos = TBN * FragPos;
}
// ==================================================================