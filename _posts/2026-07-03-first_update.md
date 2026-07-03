---
layout: post
title: "Devlog 02: First Progress Update"
date: 2026-07-03
---

Current Progress
================

Currently, we have a working asset management system, which allows us to register lambda functions to import asset types. We can then import assets from the filesystem by submitting an import command to the asset pipeline object, and then call refreshAssetManager, which imports the assets into an existing assetManager object. I feel like this is a cumbersome solution, but I'd rather leave it since it keeps import logic separate from the actual asset storage logic, and the asset importing separate from the core. This may change later.

The asset manager is also responsible for storing the assets in memory, and providing access to them. The asset manager is a singleton, and can be accessed from anywhere in the codebase. The asset manager is responsible for managing the lifetime of the assets, and will delete them when they are no longer needed.

We also have a working renderer, which can render meshes with shaders. The renderer is also responsible for managing the GPU resources, and will delete them when they are no longer needed (when the application stops).

![Imported model]({{ "/resources/images/Screenshot 2026-07-03 000753.png" | relative_url }})

Here is a screenshot of the engine rendering a model that was imported from an .obj file. The model is a simple cube, and the shader is a basic diffuse shader. The model is being rendered with a perspective camera, and a shader which just shows the normal as the color of the mesh's surface.

To Do
=====

Enitiy + Component: As I start to get more into entity management, I've been doing some research on the entity component system architecture. I found this blog post: https://austinmorlan.com/posts/entity_component_system/#the-component , which goes over a clean and simple implementation of an ECS, and I would like to implement some spin on this into my own ECS. The goal is to have readable code and architecture that doesn't bog down future development.

