## Blackhole Raytracer
This program aims to simulate Schwarschild Blackholes to a relative degree of accuracy.
All math done for this project was done with no verifaction that it was accurate other than the eye test when looking at the simulation.

## Getting Started
Add a .hdr photo to the BlackHoleTracer/Assets folder then add that file path to BlackHoleTracer/Sources/main.cpp SKYBOX_PATH variable.
If the resolution isn't big enough it may look bad.

In general keep blackhole mass relatively low (1-3) because otherwise zooming out enough to see the blackhole will cause the stepsize to be too small for light rays to reach the hole and it will stop rendering. 
```bash
cd Build
cmake -S .. -B .
...
```
## Example photos
The background is an [image of the Eagle Nebula from the ESO](https://www.eso.org/public/images/eso0926a/) that is wrapped around the blackhole. Any image could be added.

Fake Accretion Disk (on the seam of the image)
![Alt text](/../master/Examples/2026-02-17-21-11-output.png?raw=true "")
![Alt text](/../master/Examples/2026-02-17-21-14-output.png?raw=true "")
Very Small Blackhole Mass
![Alt text](/../master/Examples/2026-02-21-17-25-output.png?raw=true "")
![Alt text](/../master/Examples/2026-02-21-17-27-output.png?raw=true "")
Newtonian Acceleration of Light
![Alt text](/../master/Examples/2026-02-21-17-26-output.png?raw=true "")

## Documentationa

Functionality           | Library
----------------------- | ------------------------------------------
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
ImGui                   | [imgui](https://github.com/ocornnut/imgui)
