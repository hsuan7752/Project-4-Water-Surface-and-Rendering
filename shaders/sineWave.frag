#version 430 core

out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
   vec4 clipSpace;
   vec3 toCameraVector;
   vec3 fromLightVector;
} f_in;

uniform vec3 u_color;

uniform sampler2D u_texture;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3 lightColor;

uniform float moveFactor;

const float waveStrength = 0.02f;
const float shineDamper = 20.0f;
const float reflectivity = 0.6f;

void main()
{   
    vec2 ndc = (f_in.clipSpace.xy / f_in.clipSpace.w) / 2.0f + 0.5f;
    vec2 reflectTexCoords = vec2(ndc.x, ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);  

    float near = 0.1f;
    float far = 1000.0f;
    float depth = texture(depthMap, refractTexCoords).r;
    float floorDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

    vec2 distortedTexCoords = (texture(dudvMap, vec2(f_in.texture_coordinate.x + moveFactor, f_in.texture_coordinate.y)).rg * 2.0f - 1.0f) * waveStrength;
    distortedTexCoords = f_in.texture_coordinate + vec2(distortedTexCoords.x + moveFactor, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0f - 1.0f) * waveStrength;

    //depth = gl_FragCoord.z;
    float waterDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));
    float waterDepth = floorDistance - waterDistance;

    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001f, 0.999f);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999f, -0.001f);

    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001f, 0.999f);

    vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractionColor = texture(refractionTexture, refractTexCoords);

    vec3 viewVector = normalize(f_in.toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0f, 1.0f, 0.0f));
    //refractiveFactor = pow(refractiveFactor, 2.0f);

    //vec4 normalMapColor = texture (normalMap, distortedTexCoords);
    //vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.g, normalMapColor.b * 2.0f - 1.0f);
    //normal = normalize(normal);

    vec3 normal = f_in.normal;
    normal = normalize(normal);

    vec3 reflectedLight = reflect(normalize(f_in.fromLightVector), f_in.normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0f);
    specular = pow(specular, shineDamper);
    vec3 specularHightlights = lightColor * specular * reflectivity;

    f_color = mix(reflectionColor, refractionColor, refractiveFactor);
    f_color = mix(f_color, vec4(0.0f, 0.8f, 1.0f, 1.0f), 0.5f) + vec4(specularHightlights, 0.0f);

    //f_color = vec4(waterDepth / 50.0f);

    //vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
    //f_color = vec4(0.0f, 0.8f, 1.0f, 1.0f);
}