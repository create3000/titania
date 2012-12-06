// Based on Velvet shader from ATI RenderMonkey Fabric project
// http://developer.amd.com/gpu/rendermonkey/

varying vec3 eye_position;
varying vec3 eye_normal;
varying vec4 global_ambient;
varying vec4 light_pos;
varying vec4 light_color;
varying vec4 base_color;

const vec4 sheen = vec4(0.8137, 0.8137, 0.8137, 1.0); 
const vec4 shiny = vec4(0.1267, 0.1267, 0.1267, 1.0);
const vec4 Ka = vec4(0.23, 0.23, 0.23, 1.0);
const vec4 Kd = vec4(0.43, 0.43, 0.43, 1.0);

const float roughness = 0.1;
const float edginess = 20.5;
const float backscatter = 0.1;

vec4 diffuse(vec3 Neye, vec3 Peye)
{
   // Compute normalized vector from vertex to light in eye space  (Leye)
   vec3 Leye = (vec3(light_pos) - Peye) / length(vec3(light_pos) - Peye);

   float NdotL = dot(Neye, Leye);

   // N.L
   return vec4(NdotL, NdotL, NdotL, NdotL);
}

void main(void)
{
   vec3 Nf = normalize(eye_normal);           // Normalized normal vector
   vec3 Veye = -(eye_position / length(eye_position));  // Normalized eye vector

   // For every light do the following:

   // Hemisphere
   vec3 Leye = ( vec3(light_pos) - eye_position) / length( vec3(light_pos) - eye_position);             // Leye for a given light

   // Retroreflective lobe
   float cosine = clamp(dot(Leye, Veye), 0.0, 1.0);
   
   vec4 local_shiny = shiny + pow (cosine, 1.0 / roughness ) * backscatter * light_color * sheen;

   // Horizon scattering
   cosine     = clamp (dot(Nf, Veye), 0.0, 1.0);
   float sine = sqrt (1.0 - (cosine * cosine));
   local_shiny += pow (sine, edginess) * dot(Leye, Nf) * light_color * sheen;

   // Add in diffuse color and return
   gl_FragColor = (Ka*global_ambient + Kd*diffuse(eye_normal, eye_position)) * base_color + local_shiny;
}