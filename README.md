#OpenGL Racing Game Engine (In Progress)
Building a racing game in C++/OpenGL to explore software development techniques and learn more about implementing graphics into a functioning program.
##Built with
- OpenGL: rendering
- Bullet: physics
- Dear ImGui: debug UI
- tinyobjloader: .obj parsing
- DevIL: image loading

##Features
- ECS (Entity Component System) architecture for core gameplay logic
- Custom OpenGL renderer featuring soft shadows and pointlights
- Multiple point light support, with shadows cast from up to four sources
- .mtl file support
- Normal mapping

If more than four point lights cast shadows, the program automatically selects the four closest light sources to the camera
Vehicle physics and rigid body physics are implemented through bullet physics


https://benfoster1234567.github.io/OpenGL-Racing-Game/
