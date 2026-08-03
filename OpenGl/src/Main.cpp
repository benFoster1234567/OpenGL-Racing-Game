//#define ENABLE_TESTS
#ifndef ENABLE_TESTS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "infra/app/Application.h"
#include <infra/engine/ImportFuncs.h>

int main()
{
	auto m = Engine::Infra::ImportFuncs::importMaterialData("assets/materials/testMaterial.mtl", "testMaterial");
	std::cout << "Material Data Imported: \n";
	std::cout << "Ns: " << m.ns << "\n";
	std::cout << "Ka: " << m.ka.x << ", " << m.ka.y << ", " << m.ka.z << "\n";
	std::cout << "Kd: " << m.kd.x << ", " << m.kd.y << ", " << m.kd.z << "\n";
	std::cout << "Ks: " << m.ks.x << ", " << m.ks.y << ", " << m.ks.z << "\n";
	std::cout << "Ke: " << m.ke.x << ", " << m.ke.y << ", " << m.ke.z << "\n";
	std::cout << "Ni: " << m.ni << "\n"; 
	std::cout << "D: " << m.d << "\n";
	std::cout << "Illum: " << m.illum << "\n";
	
	Engine::Infra::Application app{};
	app.run();
    return 0;
}


#endif