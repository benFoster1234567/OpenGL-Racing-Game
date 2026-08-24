---
layout: post
title: "Devlog 03: Textures, Shadows, and Pointlights"
date: 2026-08-03
---

Renderer Progress: Shadows and Pointlights
==========================================

A singular pointlight can now cast shadows onto surfaces. I also implemented a windowing equation that I found online into my shader which attenuates light from a pointlight depending on the radius and distance.

Window(d,r) = (1 - (d/r)^4)^2 / (d^2 + 1) 

This ensures a smooth falloff instead of a sharp cutoff where the light's range ends. Not quite sure if I have range setup correctly yet, but for now I think it looks pretty good.

![Imported model]({{ "/resources/images/Screenshot 2026-08-23 161904.png" | relative_url }})

![Imported model]({{ "/resources/images/Screenshot 2026-08-23 162942.png" | relative_url }})

So far my renderer supports shadows being casted  from a single pointlight, but next time I work on it, I plan to make it so that I can designate which pointlights to cast shadows from. The method I have currently is pretty expensive, requiring a uniform array of 6 matrices to be sent to the shader on each frame for a single pointlight, so I plan on optimizing this to make it more scalable and to minimize the impact on performance.

Textures and Maps
=================

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

![Imported model]({{ "/resources/images/Screenshot 2026-08-23 162436.png" | relative_url }})

Asset Management Refactor
=========================

I refactored the asset manager and renaderer, replacing the unordered maps that contained the asset data, with sparse sets. This combined with some other reworks boosted the framerate by several hundred fps on my computer. 

My previous setup had an asset manager class with an unordererd map for each asset, with the name as a key and a unique pointer to the held asset data for the value. The in my renderer, I had unordered maps which took the raw pointer for the key and had a value of a unique pointer. To send a command to the renderer, my asset components held pointers that would be sent to the renderer, which would have to look the cached openGL up in an unordered map. This seemed pointless since the renderer only needs the asset data once, when it is imported into the gpu, so it just made more sense to use asset ID's and a sparse set instead of passing pointers around. I figured this new way would make it easier to safely manage the asset memory, reducing the risk of dangling pointers.

Current Goals
================

* Add a way to edit the game objects in the scene through some sort of config file.
* Implement pointlight shadows from multiple sources.
* Implement UI capabilities for the game.
* Migrate to a more robust build system, such as CMake, to make building the engine easier and more cross-platform.