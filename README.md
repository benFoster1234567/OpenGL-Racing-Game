OpenGL Racing Game Engine (In Progress)
=
<img width="1243" height="793" alt="image" src="https://github.com/user-attachments/assets/f4fa634e-4ba7-44a9-ab36-996279217308" />
https://github.com/user-attachments/assets/dc39f2c1-a60f-47f2-9915-6a2387ff80dc

Building a racing game in C++/OpenGL to explore software development techniques and learn more about implementing graphics into a functioning program.
## Built with
- OpenGL: rendering
- Bullet: physics
- Dear ImGui: debug UI
- tinyobjloader: .obj parsing
- DevIL: image loading

## Features
- ECS (Entity Component System) architecture for core gameplay logic
- Custom OpenGL renderer featuring soft shadows and pointlights
- Multiple point light support, with shadows cast from up to four sources
- .mtl file support
- Normal mapping

If more than four point lights cast shadows, the program automatically selects the four closest light sources to the camera
Vehicle physics and rigid body physics are implemented through bullet physics


### Normal mapping and specular mapping enabled:
<img width="1589" height="889" alt="image" src="https://github.com/user-attachments/assets/7640c248-37b3-4df7-a9c8-40dd87ef287f" />


https://benfoster1234567.github.io/OpenGL-Racing-Game/
