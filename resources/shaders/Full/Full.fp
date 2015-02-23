#version 420 core

precision highp float;

uniform mat4   mvMatrix;    // transforms from model space to view space
uniform mat4   vMatrix;     // transforms from world space to view space
uniform mat4   mMatrix;     // transforms from model space to world space

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform float normalMapping;
uniform struct Material {
    vec3 specularColor;
    float specularPower;
} material;

// ambient factor
uniform float ambientFactor;

// light position in world space
uniform vec4 lightPosition;
uniform float lightAttenuationFactor;
uniform float lightIntensity;
uniform vec3 lightColor;

uniform vec3 gammaCorrectionFactor = vec3(1.0/2.2);

// input from previous stage
in VS_OUT
{
    vec4 position;      // position of fragment in model space
    vec3 normal;        // normal vector in model space
    vec3 tangent;       // tangent vector in model space
    vec2 texCoord;      // texture coordinate
} fs_in;

float calculateLightAttenFactor()
{
    vec3 worldPos = (mMatrix * fs_in.position).xyz;
    float distance = distance(lightPosition.xyz, worldPos.xyz);
    return 1.0 / (1.0 + lightAttenuationFactor * pow(distance,2));
}

void main(void)
{
    vec3 N = normalize(mat3(mvMatrix) * fs_in.normal);
    vec3 L = normalize(
        (vMatrix * vec4(lightPosition.xyz, 1.0)).xyz - (mvMatrix * fs_in.position).xyz
    );
    vec3 V = normalize(-(mvMatrix * fs_in.position).xyz); 
    
    // recalculate vectors for normal mapping (if enabled)
    if (normalMapping != 0)
    {
        vec3 T = normalize(mat3(mvMatrix) * fs_in.tangent);
        vec3 B = cross(N, T);
    
        L = normalize(vec3(dot(L,T), dot(L,B), dot(L,N)));
        V = normalize(vec3(dot(V,T), dot(V,B), dot(V,N)));
        
        // calculate real normal from normal map
        N = normalize(texture2D(normalMap, fs_in.texCoord).rgb * 2.0 - vec3(1.0));
    }
    
    
    float lightFactor = calculateLightAttenFactor() * lightIntensity;
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, fs_in.texCoord);
    
    vec3 ambient = diffuseColor.rgb * lightColor.rgb * ambientFactor * lightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * lightColor.rgb * lightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), material.specularPower) * material.specularColor * lightColor.rgb * lightFactor; 
    
    vec3 color = ambient + diffuse + specular;
    gl_FragColor = vec4(pow(color,gammaCorrectionFactor), diffuseColor.a);
}