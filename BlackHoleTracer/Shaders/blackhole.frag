#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 camOrigin;
uniform vec3 bhPos;
uniform float bhRadius;
uniform float bhMass;

const float G = 1.0;
const float c = 1.0;
const float dt = 0.1;
const int MAX_STEPS = 1000;

vec3 NewtonianAcceleration(vec3 loc) {
    vec3 dir = bhPos - loc;
    float d2 = dot(dir, dir);
    float d3 = d2 * sqrt(d2); // Optimized dist cubed
    return G * bhMass * dir / d3;
}

void March_RK4(inout vec3 loc, inout vec3 vel, float c_dt) {
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
    
    vec3 rayDir = normalize(vec3(uv.x, uv.y, -1.0));

    vec3 loc = camOrigin;
    vec3 vel = rayDir * c;

    vec3 pixelColor = vec3(0.1, 0.1, 0.15).rgb;

    for (int i = 0; i < MAX_STEPS; i++) {
        float bhDist = length(loc - bhPos);
        
        if (bhDist < bhRadius * 1.05) {
            pixelColor = vec3(0.0, 0.0, 0.0);
            break;
        }

        if (bhDist > 50.0) {
            // Escape checkerboard
            if (sin(vel.x * 10.0) * sin(vel.y * 10.0) * sin(vel.z * 10.0) > 0.0) {
                pixelColor = vec3(0.5, 0.8, 1.0); // Light blue
            } else {
                pixelColor = vec3(0.1, 0.2, 0.4); // Dark blue
            }
            break;
        }

        float currentDt = dt * max(bhDist * 0.5, 1.0);

        March_RK4(loc, vel, currentDt);
    }

    FragColor = vec4(pixelColor, 1.0);
}