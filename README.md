OpenGL Racing Game Engine (In Progress)
=

I started my engine because I wanted to make a variation of Tron that would incorporate free turning, and arcade-style vehicle movement. When I realized the magnitude of work that this would be, especially for someone with not much C++ experience at the time, I ended up rebuilding it as a general purpose racing game engine. Over the summer I spent countless hours writing truly awful code, fixing it and learning from my mistakes. This project has since served as my sandbox for learning and implementing additional computer graphics techniques. It’s still a work in progress, but I’ve learned so much about C++ programming and fundamentals, such as the importance of cache locality, RAII, and how to write code that is extensible. I’ve had to pause it this quarter for school, and I plan to return to it after graduation. 

<img width="1243" height="793" alt="image" src="https://github.com/user-attachments/assets/f4fa634e-4ba7-44a9-ab36-996279217308" />

https://github.com/user-attachments/assets/dc39f2c1-a60f-47f2-9915-6a2387ff80dc

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

## Building the Project
Dependencies: C++20, glew, GLFW3, Bullet Physics 3, Dear ImGui, tinyobjloader, DeVIL, glm

### In Visual Studio
In project properties OpenGl/include should be added as an additional include directory
