---
layout: post
title: "Devlog 03: Third Update"
date: 2026-08-03
---

Renderer Progress : Textures and Maps
=====================================

The renderer now supports texture mapping for diffuse, ambient and specular components. I made it so that when creating a .mtl file, you can specify which texture file goes to which map. Here is an example of how the .mtl files would look:

```
# Blender MTL File: 'None'
# Material Count: 1
cubeMaterial Material
Ns 500
Ka 0.00000 0.0000 0.0000
Kd 0.0000 0.0000 0.00000
Ks 1.00000 1.00000 1.000000
Ke 0.000000 0.000000 0.000000
Ni 1.450000
d 1.000000
illum 2
map_Kd textures/pic0066.gif
map_Ks textures/pic0068.gif
```

Also, currently when a map isn't given for a map for a certain reflection color, the renderer creates a new 1x1 texture of the specified color and sends that to the shader in place of the missing map. This way, I don't need extra conditional statements inside my shaders, and I can just trust that the uploaded texture contains the correct color from the material.

