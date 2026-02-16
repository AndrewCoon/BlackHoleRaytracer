#version 400 core
out vec4 FragColor;
in vec2 TexCoord;

uniform uint flags;

uniform vec3 camPos;
uniform mat4 invView;

uniform float bhSizeBuffer;

uniform sampler2D u_skybox; 
uniform vec3 u_cameraDir;   
uniform vec3 u_cameraRight; 
uniform vec3 u_cameraUp;    
uniform float u_fov;

uniform vec3 bhPos;
uniform float bhRadius;
uniform float bhMass;

uniform float u_aspectRatio;

const float G = 1.0;
const float c = 1.0;
const float dt = 0.05;
const float PI = 3.14159265359;
const int MAX_STEPS = 10000;

vec3 NewtonianAcceleration(vec3 loc) {
    vec3 dir = bhPos - loc;
    float d2 = dot(dir, dir);
    float d3 = d2 * sqrt(d2); 
    return G * bhMass * dir / d3;
}

vec3 ToCartesian(vec3 vec) {
    float r = vec.x;
    float theta = vec.y;
    float phi = vec.z;

    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    return vec3(x, y, z);
}

vec3 GeodesicAcceleration(vec3 loc, vec3 vel) {
    float r = length(loc);
    r = max(r, 0.001);

    float factor = 1.0 - bhRadius / r;

    vec3 nLoc = loc / r;

    vec3 dVel = -1.5 * (G * bhMass / (r * r * factor)) * (dot(vel, vel) / (c * c) - factor) * nLoc;
    
    dVel += (dot(vel, nLoc) / (r * factor)) * vel;

    return dVel;
}

void March_Geodesic_RK4(inout vec3 loc, inout vec3 vel, float c_dt) {
    vec3 k1v = GeodesicAcceleration(loc, vel);
    vec3 k1x = vel;

    vec3 k2v = GeodesicAcceleration(loc + k1x * c_dt * 0.5, vel + k1v * c_dt * 0.5);
    vec3 k2x = vel + k1v * c_dt * 0.5;

    vec3 k3v = GeodesicAcceleration(loc + k2x * c_dt * 0.5, vel + k2v * c_dt * 0.5);
    vec3 k3x = vel + k2v * c_dt * 0.5;

    vec3 k4v = GeodesicAcceleration(loc + k3x * c_dt, vel + k3v * c_dt);
    vec3 k4x = vel + k3v * c_dt;

    loc += (c_dt / 6.0) * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
    vel += (c_dt / 6.0) * (k1v + 2.0 * k2v + 2.0 * k3v + k4v);
    
    vel *= c / length(vel);
}

void March_Newtonian_RK4(inout vec3 loc, inout vec3 vel, float c_dt) {
    vec3 k1v = NewtonianAcceleration(loc);
    vec3 k1x = vel;

    vec3 k2v = NewtonianAcceleration(loc + c_dt / 2.0 * k1x);
    vec3 k2x = vel + c_dt / 2.0 * k1v;

    vec3 k3v = NewtonianAcceleration(loc + c_dt / 2.0 * k2x);
    vec3 k3x = vel + c_dt / 2.0 * k2v;

    vec3 k4v = NewtonianAcceleration(loc + c_dt * k3x); 
    vec3 k4x = vel + c_dt * k3v;

    vel += c_dt / 6.0 * (k1v + 2.0 * k2v + 2.0 * k3v + k4v);
    vel = normalize(vel) * c;

    loc += c_dt / 6.0 * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
}

vec2 DirectionToUV(vec3 dir) {
    float phi = atan(dir.z, dir.x);
    float theta = asin(dir.y);
    
    float u = 1.0 - (phi + PI) / (2.0 * PI);
    float v = theta / PI + 0.5;
    
    return vec2(u, v);
}

void main() {
    bool useRelativity = (flags & (1u << 0)) != 0u;
    bool showDisk = (flags & (1u << 1)) != 0u;

    vec2 ndc = TexCoord * 2.0 - 1.0;
    ndc.x *= u_aspectRatio;

    float fovFactor = tan(u_fov * 0.5);
    vec3 rayDirCam = normalize(vec3(ndc.x * fovFactor, ndc.y * fovFactor, -1.0));
    vec3 rayDir = normalize((invView * vec4(rayDirCam, 0.0)).xyz);
    
    vec3 loc = camPos;
    vec3 vel = rayDir * c;

    vec3 pixelColor = vec3(1.0);

    for (int i = 0; i < MAX_STEPS; i++) {
        float bhDist = length(loc - bhPos);

        float diskInner = bhRadius * 2;
        float diskOuter = bhRadius * 6.0;

        if (abs(loc.y - bhPos.y) < (0.01) && showDisk) {
            if (bhDist > diskInner && bhDist < diskOuter) {
                float t = (bhDist - diskInner) / (diskOuter - diskInner);
                vec3 diskColor = mix(vec3(1.0, 0.7, 0.2), vec3(0.5, 0.1, 0.0), t);
                
                // Simple gravitational redshift for the disk
                float redshift = sqrt(max(1.0 - bhRadius / bhDist, 0.01));
                pixelColor = diskColor * redshift * 2.0;
                break;
            }
        }

        if (bhDist < bhRadius * bhSizeBuffer) {
            pixelColor = vec3(0.0);
            break;
        }

        if (bhDist > 100.0) {
            vec2 skyUV = DirectionToUV(normalize(vel));
            pixelColor = texture(u_skybox, skyUV).rgb;

            float redshift = sqrt(max(1.0 - bhRadius / bhDist, 0.05));
            pixelColor *= redshift;
            break;
        }

        float currentDt = dt * clamp(bhDist * 0.5, 0.1, 10.0);
        if  (useRelativity) {
            March_Geodesic_RK4(loc, vel, currentDt);
        } else {
            March_Newtonian_RK4(loc, vel, currentDt);
        }
    }

    pixelColor = pixelColor / (pixelColor + vec3(1.0));
    pixelColor = pow(pixelColor, vec3(1.0 / 2.2));

    FragColor = vec4(pixelColor, 1.0);
}