// Based on Point Light shader from oZone3D.Net 
// http://www.ozone3d.net/tutorials/glsl_lighting_phong_p2.php

varying vec3 normal, light_direction, eye_position;
varying float attenuation;

void main(void)
{	
  normal = gl_NormalMatrix * gl_Normal;

  vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
  light_direction = vec3(gl_LightSource[0].position.xyz - vVertex);
  eye_position = -vVertex;
	
  float d = length(light_direction);
	
  attenuation = 1.0 / ( gl_LightSource[0].constantAttenuation + 
                (gl_LightSource[0].linearAttenuation*d) + 
                (gl_LightSource[0].quadraticAttenuation*d*d) );

  gl_Position = ftransform();		
}
