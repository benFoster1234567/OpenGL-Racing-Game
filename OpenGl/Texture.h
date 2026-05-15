#pragma once
#include <GL/glew.h>
#include <IL/il.h>

#include <string>

class Texture
{
private:
	unsigned int id;
	std::string type;
	std::string path;
public :
	Texture();
	void Bind();
	unsigned int GetID() const { return id; }
	std::string GetType() const { return type; }
	std::string GetPath() const { return path; }
	std::string GetName() const { return path.substr(path.find_last_of("/\\") + 1); }

	unsigned int loadTexture(const char* filename);
};

