#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexNormals;
layout(location=2) in vec2 texCoords; // Added texture coordinates

// Pass to fragment shader
out vec3 v_vertexNormals;
out vec2 v_texCoords; // Passed to fragment shader
out vec3 FragPos;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main()
{
  v_vertexNormals = vertexNormals;
  v_texCoords = texCoords; // Pass the texture coordinates

  FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));

  gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
}
