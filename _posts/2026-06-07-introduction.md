---
layout: post
title: "Devlog 01: Where We Are and Where We're Going!"
date: 2026-06-07
---
Hi, welcome to the first devlog of my motorcycle racing game. I am a soon-to-be computer science graduate, and the purpose of this project is to have fun, gain experience designing software, and to get better with C++ and OpenGL.

From Humble Beginnings
======================

My game began with me wanting to create a Tron clone, but it quickly evolved into me wanting to create a motorcycle racing game with simple physics and more nuanced collision mechanics. 

The intitial design of my project was to separate the core game logic from the physics libraries, rendering library, and the window library. These core mechanics would live in the core, while OpenGl would be wrapped in its own renderer class. Glfw would be wrapped in its own window class, and a physics library would be wrapped in its own class, acting as a service, where everytime a collision occurs, the physics service would send the collision data to the entity class, which would react to it. There would also need to be raycasting, which would likely also be wrapped. As someone who has never used a C++ physics library before, I decided to sidebar this until I had a working renderer to visualize the ways different objects interacted. So I essentially created this layout tree diagram using an online tool:

```
My current file tree:
.
└── OpenGl/
    ├── include/
    │   ├── core/
    │   │   ├── assets/
    │   │   │   ├── AssetManager.h
    │   │   │   ├── AssetPipeline.h
    │   │   │   ├── MeshData.h
    │   │   │   ├── TextureData.h
    │   │   │   ├── MaterialData.h
    │   │   │   └── ShaderData.h
    │   │   ├── ecs/
    │   │   │   ├── Entity.h
    │   │   │   ├── EntityManager.h
    │   │   │   └── EntityController.h
    │   │   ├── engine/
    │   │   │   └── Engine.h
    │   │   ├── events/
    │   │   │   ├── EventDispatcher.h
    │   │   ├── input/
    │   │   │   └── InputHandler.h
    │   │   └── physics/
    │   │       └── CollisionBehavior.h
    │   └── infra/
    │       ├── app/
    │       │   ├── Application.h
    │       │   ├── GlfwKeyHandler.h
    │       │   └── Window.h
    │       ├── engine/
    │       │   ├── AssetImportFuncs.h
    │       │   └── KeyLookup.h
    │       └── renderer/
    │           ├── Renderer.h
    │           └── assets/
    │               ├── GpuTexture.h
    │               ├── GpuShader.h
    │               └── GpuMesh.h
    └── src/
        ├── core/
        │   └── assets/
        │       ├── AssetManager.cpp
        │       ├── AssetPipeline.cpp
        │       ├── MeshData.cpp
        │       ├── TextureData.cpp
        │       ├── MaterialData.cpp
        │       ├── ShaderData.cpp
        │       ├── ecs/
        │       │   ├── Entity.cpp
        │       │   ├── EntityManager.cpp
        │       │   └── EntityController.cpp
        │       ├── engine/
        │       │   └── Engine.cpp
        │       ├── events/
        │       │   ├── EventDispatcher.cpp
        │       ├── input/
        │       │   └── InputHandler.cpp
        │       └── physics/
        │           └── CollisionBehavior.cpp
        └── infra/
            ├── app/
            │   ├── Application.cpp
            │   ├── GlfwKeyHandler.cpp
            │   └── Window.cpp
            ├── engine/
            │   ├── AssetImportFuncs.cpp
            │   └── KeyLookup.cpp
            └── renderer/
                ├── Renderer.cpp
                └── asset/
                    ├── GpuTexture.cpp
                    ├── GpuShader.cpp
                    └── GpuMesh.cpp
```

The first system I implemetnted was the input system, utilizing an event mechanism similar to C#'s action delegates. I named the subject class EventDispatcher. I wanted to be able to subscribe methods to an event designated for a key, which would be called from my window class using keyEvent.invoke. I implemented this in an InputHandler class where each key had its own event dispatcher, which was stored in multiple hash maps. We'd have one hashmap for the raw keycodes, and another for the actual controls (go left, go right, etc.). I tested this and it pretty much worked, so I decided to move on. 


Engine Core Classes
===================

Entity Component System
=======================
The entity-component-system is a concept used by game engines like Unity, where each game object is an entity, and each entity is composed of different components which determine the object's behavior. With my engine, I opted for a similar system, but a bit more strict.

The Entity Class is composed of a pointer to the MeshData object containing the vertex data for its mesh, a pointer to a collision behvior object, which would determine what how the entity reacts to a collision, a material data object, which contains pointers to the texture assets and other material data, an EntityController object, which controls the entity's movement outside of physics, and a pointer to a ShaderData class, which determines which shader to use. The EntityManager owns the entities, and controls an entity render queue, which sends the entity data to the renderer.

AssetManager and AssetPipeline
==============================

The AssetManager holds all of the asset data (MeshData, TextureData, MaterialData, etc) as unique pointers in maps, allowing me to reuse the same memory every time an object is from the same asset files. This was to ensure that there wouldn't be any duplicated data. For example, if I wanted two entities that used the same model, they would use the same data, and we wouldn't end up wasting memoory by importing the same asset twice. However, by keeping the asset manager as its own unique class, this opens up the opportunity for me to go back and add virtualization if my asset pool grows too large. Also by using unique pointers for the memory storage, this would ensure that all memory is cleaned when the application closes or goes out of scope. This would also mean that the Entities that reference those assets would need to be destroyed prior to the Application leaving to the scope to prevent dangling pointers. All the Entity would have to render would be to send the data to the renderer, which would wrap the pointer (if not already wrapped) in a GPU-friendly pointer, and add it to a queue to be rendered.

The AssetPipeline is responsible for importing the objects. This is a point of friction in my project though because I wanted to keep the AssetPipeline in the core. Essentially I made a method in the AssetPipeline that takes a reference to the AssetManager as an argument, and builds it from the ground up. The AssetPipeline ended up evolving into something needlessly complex. The implementation itself is not finalized. 


Where I'm at now: Creating a Window
===================================
After finishing my initial iteration of the input system, I moved on to the window system. Multiple sources on the internet recommended GLFW as the go-to window library for game development. Having used freeglut for my college classes, I found GLFW to be much better due to it's simplicity and how it offered more freedom than glut. I liked how it also allowed the user to implement a game loop instead of relying on the timers from glut. So after wrapping glfw in its own window class, I was able to easily render a blank window. I used the glfw example in the docs to do this.

I recently added a debugger console and a command system which maps a list of strings to function arguments and allows me as the programmer to register methods into the debug console that I can call from the debug console. The first command added was exit, which just closes the program. 



```cpp
namespace Engine::Infra
{

	class Application
	{
	private:
		Infra::Renderer renderer;
		Engine::Core::Engine engine;
		std::unique_ptr<DebugConsoleUi> debugConsoleUi;
		std::unique_ptr<Window> window;

	public:

		Application();
		~Application() = default;
		
		Core::AssetPipeline assetPipeline;
		
		void setupDebugCommands();
		void setupImportCallbacks();
		void importAssets();

		void setupInput();
		void processInput();
		void updateGameLogic();
		void updateRenderQueue();

		void run();
	};

}
```

My window is visible, and utilizing google, and some glfw example docs, I was able to have glfw automatically detect my screen dimensions, and launch in fullscreen. However, the window doesn't always behave correctly, often giong black and no longer showing the actual grey screen or the console, so I was unable to use snipping tool to take a screenshot. I'm wondering if maybe my render loop is out of order, or if the 

What is Next:
=============

The window needs to be fixed before I can move onto actually rendering an image, so I will fix the window next, and possibly add commands to change the resolution, and to toggle fullscreen.

Essentially my next main goal is to get an entity rendering on-screen, so I'll also need to implement some sort of camera entity.