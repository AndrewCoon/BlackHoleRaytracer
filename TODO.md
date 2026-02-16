# ToDo

Core Infrastructure

    [x] Integrate Dear ImGui (Setup GLFW/OpenGL3 backends)

    [x] Create Debug UI Window (Sliders for bhMass, bhPos, and stepSize)

    [x] Camera Orbit (Middle-mouse drag)
    
    [x] Camera Zoom (Scroll wheel)
    
    [ ] Camera Pan (Shift-drag)

    [x] FPS Monitor (Real-time frame time display)

    [ ] Step Count Display (Show ray-march iteration count)

    [ ] Add button to save current frame to image

Visual Environment

    [ ] HDR Skybox Implementation (Switch from checkerboard to equirectangular star map)

    [ ] Spacetime Curvature Grid (World-space coordinate mapping in shader)

    [ ] Second Light-Emitting Object (Implement a movable sphere/point light)

    [ ] 3D Scene Objects (Add simple geometric shapes to observe gravitational lensing)

Black Hole Features

    [x] Accretion Disk Geometry (Define inner/outer radii and disk thickness)

    [ ] Disk Texture/Noise (Procedural animated noise for hot gas appearance)

    [ ] Volumetric Disk Rendering (Integrate disk density along the ray path)

    [ ] Shadow of the Event Horizon (Refine the "photon sphere" boundary)

Advanced Physics (Relativity)

    [x] Gravitational Redshift (Shift light frequency based on Rs​/r)

    [ ] Relativistic Beaming (Adjust disk brightness based on velocity toward/away from camera)

    [ ] Doppler Shift (Color-shift disk pixels based on orbital velocity)

    [ ] Secondary/Tertiary Images (Optimize steps to capture light loops around the BH)