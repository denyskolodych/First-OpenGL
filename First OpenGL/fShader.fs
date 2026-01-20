#version 330 core

in vec3 Result;
out vec4 FragColor;
  
uniform vec3 objectColor;


void main()
{

    vec3 result = Result * objectColor;
    FragColor = vec4(result, 1.0);
}