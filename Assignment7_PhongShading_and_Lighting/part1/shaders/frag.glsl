#version 410 core

in vec3 v_vertexColors;
in vec3 v_vertexNormals;
in vec3 FragPos;

out vec4 color;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLight;

uniform vec3 u_CameraPosition; // Add this uniform at the top

// Entry point of program
void main()
{
    vec3 norm = normalize(v_vertexNormals);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 ambient = pointLight.ambient * v_vertexColors;
    vec3 diffuse = diff * pointLight.diffuse * v_vertexColors;

    vec3 viewDir = normalize(u_CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * pointLight.specular * v_vertexColors;

    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    vec3 result = (ambient + attenuation * (diffuse + specular));

    color = vec4(result, 1.0f);
	// color = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);
    // color = vec4((v_vertexNormals + 1.0) * 0.5, 1.0f);
}