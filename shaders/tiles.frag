#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} f_in;

in vec3 vs_normal;

uniform vec3 u_color;

uniform sampler2D u_texture;

void main()
{   
    vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
    //if (vs_normal.z > 0) 
    //    discard;
    //else
        f_color = vec4(color, 1.0f);

        //if (f_in.normal.y < 0)
        //    discard;
}