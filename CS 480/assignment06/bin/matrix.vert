attribute vec3 v_position;
attribute vec2 v_tex;

varying vec2 tex_coords;
varying vec4 pos_coords;

uniform mat4 mvpMatrix;

void main(void)
{
   gl_Position = mvpMatrix * vec4(v_position, 1.0);
   tex_coords = v_tex;
}
