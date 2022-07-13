#ifndef GLOBALS_H
#define GLOBALS_H

#include "Camera.h"
#include "House.h"
#include "Cube.h"
#include "Sphere.h"
#include "OBJObjectTexture.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "Window.h"

class Globals
{
    
public:
    
    static Camera camera;

	static House house;

    static Cube cube;

	static Sphere sphere;

	//static OBJObject bunny, bear;
	static OBJObjectTexture bunny, dragon;

    static Light light, directional, point, spot;

    static DrawData drawData;
    static UpdateData updateData;
    //Feel free to add more member variables as needed
    
};

#endif
