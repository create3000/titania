// Based on Velvet shader from ATI RenderMonkey Fabric project
// http://developer.amd.com/gpu/rendermonkey/

varying vec3 eye_position;
varying vec3 eye_normal;
varying vec4 global_ambient;
varying vec4 light_pos;
varying vec4 light_color;
varying vec4 base_color;

void main(void)
{
  gl_Position = ftransform();
   
  eye_position = vec3(gl_ModelViewMatrix * gl_Vertex);
  eye_normal = gl_NormalMatrix * gl_Normal;
  global_ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
  light_pos = gl_LightSource[0].position;
  light_color = gl_LightSource[0].diffuse;
  base_color = gl_FrontMaterial.diffuse;
}
