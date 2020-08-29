#include <iostream> 
#include <assert.h>
#include <thread>

#include "Screen.h"
#include "Raytracing.h"
#include "Pathtracing.h"

using namespace std;

const vec3 vec3::zero(0., 0., 0.);
const vec3 vec3::one(1., 1., 1.);
const vec3 vec3::X(1., 0., 0.);
const vec3 vec3::Y(0., 1., 0.);
const vec3 vec3::Z(0., 0., 1.);

Screen* screen = nullptr;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd) 
{
	screen = new Screen();

	srand(uint32_t(time(0)));
	cam.init();

	thread rd(RayTracing::render);
	// thread rd(PathTracing::render);

    uint32_t size = SIZE;
	int ret = screen->init(size, size, "Raytracing");
	if (ret < 0) {
		exit(ret);
	}

	while (!screen->isExit()) {
		screen->dispatch();
		for (UINT32 i = 0; i < size; i++)
			for (UINT32 j = 0; j < size; j++)
			{
				screen->setColor(i, j, framebuffer[i][j]);
			}

		screen->update();
		Sleep(1);
	}

	rd.detach();

	return 0;
}