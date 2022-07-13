#include "Globals.h"

Camera Globals::camera;

House Globals::house;

Cube Globals::cube(10.0);

Sphere Globals::sphere(10.0, 50, 50);

OBJObjectTexture Globals::bunny("Oshus (Whale).obj", 2);
OBJObjectTexture Globals::dragon("Going Merry.obj", 3);
//OBJObject Globals::bear ("bear.obj", 4);

Light Globals::light, Globals::directional, Globals::point, Globals::spot;

DrawData Globals::drawData;
UpdateData Globals::updateData;