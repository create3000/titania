// Based on Point Light shader from oZone3D.Net 
// http://www.ozone3d.net/tutorials/glsl_lighting_phong_p2.php

varying vec3 normal, light_direction, eye_position;
varying float attenuation;

void main(void)
{
  vec4 final_color = 
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient)*attenuation;
							
  vec3 N = normalize(normal);
  vec3 L = normalize(light_direction);
	
  float lambert_term = dot(N,L);
	
  if(lambert_term > 0.0)
	{
      final_color += gl_LightSource[0].diffuse * 
                     gl_FrontMaterial.diffuse * lambert_term * attenuation;	
		
      vec3 E = normalize(eye_position);
      vec3 R = reflect(-L, N);
		
      float specular = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
		
      final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * 
                     specular * attenuation;	
	}

	gl_FragColor = final_color;			
}
