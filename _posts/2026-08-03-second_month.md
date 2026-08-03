---
layout: post
title: "Devlog 03: Second Month Update"
date: 2026-08-03
---

Entity Component System
==========================

First things first, I have implemented a basic version of the entity component system architecture heavily inspired by Austin Morlan's implementation, though I may go back through and modify it further. The ECS makes creating and managing game objects such as lights and shaders super easy. One issue I've had is figuring out a clean way to export objects to the renderer. So far I've been using a static event in render dispatcher system class, which seems a bit awkward, so I might fix this later with a refactor.

Blinn-Phong Shader
=====================

After implementing lights into my renderer via a uniform buffer object, I was able to implement a basic Blinn-Phong shader. So far there are no shadows, but I plan to implement these later. Initially after writing the shader I noticed some horrible banding. I mitigated mostly mitigated this using a simple noise algorithm, which I will likely replace with a better solution later. The shader is also currently using solid colors in place of diffuse textures, but I plan to implement textures, normal mapping and specular mapping later.

![Imported model]({{ "/resources/images/Screenshot 2026-08-03 140816.png" | relative_url }})