#version 330 core

//out vec4 FragColor;

//in vec3 ourColor;

uniform float u_time;

void main()
{
    gl_FragColor = vec4(abs(sin(u_time)),0.0,0.0,1.0);
}