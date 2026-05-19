#pragma once
#include <IL/il.h>

#include <string>

class TextureData
{
private:
	std::string type;
	std::string path;
public :
	TextureData();

	std::string getType() const { return type; }
	std::string getPath() const { return path; }
	std::string getName() const { return path.substr(path.find_last_of("/\\") + 1); }

};

