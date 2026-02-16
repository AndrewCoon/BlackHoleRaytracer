#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 camPos;
uniform mat4 invView;

uniform vec3 bhPos;
uniform float bhRadius;
uniform float bhMass;

uniform float u_aspectRatio;

const float G = 1.0;
const float c = 1.0;
const float dt = 0.05;
const int MAX_STEPS = 10000;

vec3 NewtonianAcceleration(vec3 loc) {
    vec3 dir = bhPos - loc;
    float d2 = dot(dir, dir);
    float d3 = d2 * sqrt(d2); // Optimized dist cubed
    return G * bhMass * dir / d3;
}

vec3 PosToSpherical(vec3 pos) {
    float r = length(pos);
    float theta = acos(pos.z / r);
    float phi = atan(pos.y, pos.x);

    return vec3(r, theta, phi);
}

vec3 VelToSpherical(vec3 vel, vec3 pos) {
    float r = length(pos);
    
    float vr = (vel.x * pos.x + vel.y * pos.y + vel.z * pos.z) / r;
    float theta = (pos.x * vel.x + pos.y * vel.y) / r;
    float phi = (-1.0 * pos.x * vel.x + pos.y * vel.y) / (r  * sin(theta));

    return vec3(r, theta, phi);
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

void main() {
    vec2 uv = TexCoord * 2.0 - 1.0;
    uv.x *= u_aspectRatio;

    vec3 rayDirCam = normalize(vec3(uv.x, uv.y, -1.0));
    
    vec3 rayDir = normalize((invView * vec4(rayDirCam, 0.0)).xyz);
    
    vec3 loc = camPos;
    vec3 vel = rayDir * c;

    vec3 pixelColor = vec3(1.0, 0.1, 0.15).rgb;

    for (int i = 0; i < MAX_STEPS; i++) {
        float bhDist = length(loc - bhPos);

        float diskInner = bhRadius * 1.5;
        float diskOuter = bhRadius * 6.0;
        float r = length(loc - bhPos);

        if (abs(loc.y - bhPos.y) < 0.02) {
            if (r > diskInner && r < diskOuter) {
                float t = (r - diskInner) / (diskOuter - diskInner);
                vec3 diskColor = mix(vec3(1.0,0.8,0.3), vec3(1.0,0.2,0.0), t);
                pixelColor = diskColor;
                float redshift = sqrt(max(1.0 - bhRadius / r, 0.0));
                pixelColor *= redshift;
                break;
            }
        }

        if (bhDist < bhRadius * 1.05) {
            pixelColor = vec3(0.0, 0.0, 0.0);
            break;
        }

        if (bhDist > 50.0) {
            if (sin(vel.x * 10.0) * sin(vel.y * 10.0) * sin(vel.z * 10.0) > 0.0) {
                pixelColor = vec3(0.1, 0.4, 0.4); // Light blue
            } else {
                pixelColor = vec3(0.05, 0.0, 0.2); // Dark blue
            }

            // vec3 dir = normalize(vel);
            // vec2 sphereUV = vec2(
            //     atan(dir.y, dir.x) / (2.0 * 3.1415926) + 0.5,
            //     acos(dir.z) / 3.1415926
            // );

            float redshift = sqrt(max(1.0 - bhRadius / r, 0.0));
            pixelColor *= redshift;
            break;
        }

        float currentDt = dt * clamp(bhDist * 0.2, 0.02, 5.0);

        // March_Newtonian_RK4(loc, vel, currentDt);
        March_Geodesic_RK4(loc, vel, currentDt);
    }

    FragColor = vec4(pixelColor, 1.0);
}