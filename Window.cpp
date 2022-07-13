#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include "math.h"

#include "Window.h"
#include "House.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"

int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here

float spinDirection = 1;    //Changes the direction of the cube
int Window::flag = 0;               //Indicates when to switch from cube to house
int rendering = 1;          //Indicates which rendering engine to use

int frame = 0, gtime = 0, timebase = 0, showFPS = 1;   //Used in calculating frame rate
float fps = 0;							  //Used in calculating frame rate

int Window::object = 0;   //Indicates which object we are moving in window
bool Rotate = false, Move = false;  //Whether we rotate object around axis or move around window
Vector3 lastPoint;   //Used in trackballmapping

float Window::clickedX = 0;   //Used for spotlight
float Window::clickedY = 0;   //Used for spotlight
int spot = 1;

Sphere pointLight(0.5, 50, 50);
Sphere spotLight(0.5, 50, 50);

void Window::initialize(void)
{
    //Setup the light
   /* Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;*/
	Globals::directional.position = Vector4(0.0, 10.0, 15.0, 0.0);
	//Globals::directional.ambientColor = Color::white();
	//Globals::directional.diffuseColor = Color::white();
	//Globals::directional.specularColor = Color::white();

	Globals::point.position = Vector4(2.0, 2.0, 2.0, 1.0);
	Globals::point.ambientColor = Color::white();
	Globals::point.diffuseColor = Color::white();
	Globals::point.specularColor = Color::white();

	Globals::spot.position = Vector4(-2.0, 2.0, 2.0, 1.0);
	Globals::spot.ambientColor = Color::white();
	Globals::spot.diffuseColor = Color::white();
	Globals::spot.specularColor = Color::white();
	Globals::spot.exponent = 64.0;
	Globals::spot.angle = 45.0;

	Matrix4 trans;

	trans.makeTranslate(2.0, 2.0, 2.0);
	pointLight.toWorld = trans * pointLight.toWorld;

	trans.makeTranslate(-2.0, 2.0, 2.0);
	spotLight.toWorld = trans * spotLight.toWorld;
    
    //Initialize cube matrix:
    //Globals::cube.toWorld.identity();
	Globals::sphere.toWorld.identity();
    
    //Setup the cube's material properties
    //Color color(0x23ff27ff);
    //Globals::cube.material.color = color;
	//Globals::sphere.material.color = color;
	for (unsigned int i = 0; i < Globals::bunny.materials.size(); i++) {
		Globals::bunny.materials[i].loadTexture();
	}


	for (unsigned int i = 0; i < Globals::dragon.materials.size(); i++) {
		Globals::dragon.materials[i].loadTexture();
	}
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	//Calculates the frame rate of an object
	if (showFPS == -1)
		calculateFPS();

    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
    //Rotate cube; if it spins too fast try smaller values and vice versa
    //Globals::cube.spin(spinDirection * 0.0005);
    
    //Call the update function on cube
    //Globals::cube.update(Globals::updateData);
	Globals::sphere.update(Globals::updateData);
    
    //Call the display routine to draw the cube
	if (rendering == -1) {
	}
	else {
		displayCallback();
	}
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	Globals::camera.update();

    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    Globals::directional.bind(0);
	Globals::point.bind(1);
	Globals::spot.bind(2);

	pointLight.draw(Globals::drawData);
	spotLight.draw(Globals::drawData);
    
    //Draw cube or house
	switch (flag) {
	case 0:
		Globals::sphere.material.ambientColor = Color(0.135f, 0.2225f, 0.1575f);
		Globals::sphere.material.diffuseColor = Color(0.54f, 0.89f, 0.63f);
		Globals::sphere.material.specularColor = Color(0.316228f, 0.316228f, 0.316228f);
		Globals::sphere.material.shininess = 30.0;

		Globals::sphere.draw(Globals::drawData);
		break;

	case 1:
		Globals::house.draw(Globals::drawData);
		break;

	case 2:
		/*Globals::bunny.material.ambientColor = Color(0.24725, 0.1995, 0.0745);
		Globals::bunny.material.diffuseColor = Color(0.75164, 0.60648, 0.22648);
		Globals::bunny.material.specularColor = Color(0.628281, 0.555802, 0.366065);
		Globals::bunny.material.shininess = 255.0;*/

		Globals::bunny.draw(Globals::drawData);
		break;

	case 3:
		//Globals::dragon.material.ambientColor = Color(0.0, 0.0, 0.5);
		//Globals::dragon.material.diffuseColor = Color(0.8, 0.8, 0.8);
		//Globals::dragon.material.specularColor = Color(0.2, 0.2, 0.2);
		//Globals::dragon.material.shininess = 30.0;

		Globals::dragon.draw(Globals::drawData);
		break;

	case 4:
		/*Globals::bear.material.ambientColor = Color(0.1745, 0.01175, 0.01175);
		Globals::bear.material.diffuseColor = Color(0.61424, 0.04136, 0.04136);
		Globals::bear.material.specularColor = Color(0.727811, 0.626959, 0.626959);
		Globals::bear.material.shininess = 255.0;

		Globals::bear.draw(Globals::drawData);*/
		break;
	}
    
    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}

//Calculates frame rate of object in Window
void Window::calculateFPS()
{
	//  Increase frame count
	frame++;

	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	gtime = glutGet(GLUT_ELAPSED_TIME);

	//  Calculate time passed
	int timeInterval = gtime - timebase;

	if (timeInterval > 1000)
	{
		//  calculate the number of frames per second
		fps = frame / (timeInterval / 1000.0f);

		//  Set time
		timebase = gtime;

		//  Reset frame count
		frame = 0;

		std::cout << "FPS: " << fps << std::endl;
	}
}

//TODO: Keyboard callbacks!
void Window::keyboard(unsigned char key, int x, int y) {
	Matrix4 trans;

	switch (key) {
	/*case 'c':
		//if (flag == 0)
			//spinDirection = -spinDirection;

		break;

	case 'x':
		trans.makeTranslate(-1, 0, 0);
		moveObject(trans);
		break;

	case 'X':
		trans.makeTranslate(1, 0, 0);
		moveObject(trans);
		break;

	case 'y':
		trans.makeTranslate(0, -1, 0);
		moveObject(trans);
		break;

	case 'Y':
		trans.makeTranslate(0, 1, 0);
		moveObject(trans);
		break;

	case 'z':
		trans.makeTranslate(0, 0, -1);
		moveObject(trans);
		break;

	case 'Z':
		trans.makeTranslate(0, 0, 1);
		moveObject(trans);
		break;*/

	case 'r':
		switch (flag) {
		case 0:
			Globals::sphere.toWorld.identity();
			//spinDirection = 1;
			break;

		case 2:
			Globals::bunny.resetObject();
			break;

		case 3:
			Globals::dragon.resetObject();
			break;

		case 4:
			//Globals::bear.resetObject();
			break;
		}

		break;

	/*case 'o':
		trans.makeRotateZ(10 * M_PI / 180);
		moveObject(trans);
		break;

	case 'O':
		trans.makeRotateZ(-10 * M_PI / 180);
		moveObject(trans);
		break;

	case 's':
		trans.makeScale(.9);
		scaleObject(trans);
		break;

	case 'S':
		trans.makeScale(1.1);
		scaleObject(trans);
		break;

	case 'e':
		rendering = -rendering;
		break;

	case '+':
		if (rendering == -1 && Globals::rasterizer.part < 3)
			Globals::rasterizer.part++;

		break;

	case '-':
		if (rendering == -1 && Globals::rasterizer.part > 0)
			Globals::rasterizer.part--;
		
		break;

	case 'd':
		if (rendering == -1 && Globals::rasterizer.part != 0)
			Globals::rasterizer.box = -Globals::rasterizer.box;
			
		break;*/

	case 's':
		if (object == 3) {
			spot = -spot;
			if (spot == 1)
				std::cout << "Switched to spotlight angle" << std::endl;

			else
				std::cout << "Switched to spotlight exponent" << std::endl;
		}

		break;

	case 'f':
		showFPS = -showFPS;
		break;

	case '0':
		object = 0;
		break;

	case '1':
		object = 1;
		break;

	case '2':
		object = 2;
		break;

	case '3':
		object = 3;
		break;
	}

	/*if (flag == 0) {
		Vector3 position(Globals::cube.toWorld.get(3, 0), Globals::cube.toWorld.get(3, 1),
			Globals::cube.toWorld.get(3, 2));

		position.print("Position of Cube");
	}*/
}

//Moves the object in Window depeding on which key is pressed
void Window::moveObject(Matrix4 translate) {
	switch (flag) {
	case 0:
		Globals::sphere.toWorld = translate * Globals::sphere.toWorld;
		break;

	case 2:
		Globals::bunny.toWorld = translate * Globals::bunny.toWorld;
		break;

	case 3:
		Globals::dragon.toWorld = translate * Globals::dragon.toWorld;
		break;

	case 4:
		//Globals::bear.toWorld = translate * Globals::bear.toWorld;
		break;
	}
}

//Scales the object in Window
void Window::scaleObject(Matrix4 scale) {
	switch (flag) {
	case 0:
		Globals::sphere.toWorld = Globals::sphere.toWorld * scale;
		break;

	case 2:
		Globals::bunny.toWorld = Globals::bunny.toWorld * scale;
		break;

	case 3:
		Globals::dragon.toWorld = Globals::dragon.toWorld * scale;
		break;

	case 4:
		//Globals::bear.toWorld = Globals::bear.toWorld * scale;
		break;
	}
}

//TODO: Function Key callbacks!
void Window::specialKeyboard(int key, int x, int y) {
	Vector3 e, d, up;

	switch (key) {
	case GLUT_KEY_F1:
		flag = 0;
		Globals::camera = Camera();
		//Globals::rasterizer.toDrawOBJ = &Globals::cube;
		break;

	/*case GLUT_KEY_F2:
		e.set(0.0, 24.14, 24.14);
		d.set(0.0, 0.0, 0.0);
		up.set(0.0, 1.0, 0.0);
		Globals::camera.set(e, d, up);
		flag = 1;
		Globals::rasterizer.toDrawOBJ = &Globals::house;
		break;

	case GLUT_KEY_F3:
		e.set(-28.33, 11.66, 23.33);
		d.set(-5.0, 0.0, 0.0);
		up.set(0.0, 1.0, 0.5);
		Globals::camera.set(e, d, up);
		flag = 1;
		Globals::rasterizer.toDrawOBJ = &Globals::house;
		break;*/

	case GLUT_KEY_F2:
		flag = 2;
		Globals::camera = Camera();
		//Globals::rasterizer.toDrawOBJ = &Globals::bunny;
		break;

	case GLUT_KEY_F3:
		flag = 3;
		Globals::camera = Camera();
		//Globals::rasterizer.toDrawOBJ = &Globals::dragon;
		break;

	case GLUT_KEY_F4:
		flag = 4;
		Globals::camera = Camera();
		//Globals::rasterizer.toDrawOBJ = &Globals::bear;
		break;
	}
}

Vector3 Window::trackBallMapping(int x, int y)
{
	// Vector v is the synthesized 3D position of the mouse location on the trackball
	Vector3 v;

	// this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	float d;
	
	// this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v[0] = (2.0f * x - float(width)) / float(width);
	
	// this does the equivalent to the above for the mouse Y position
	v[1] = (float(height) - 2.0f * y) / float(height);
	
	// initially the mouse z position is set to zero, but this will change below
	v[2] = 0.0;
	
	// this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = v.magnitude();
	
	// this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	d = (d < 1.0f) ? d : 1.0f;
	
	// this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v[2] = sqrtf(1.001f - d * d);
	
	// Still need to normalize, since we only capped d, not v.
	v = v.normalize();
	
	// return the mouse location on the surface of the trackball
	return v;
}

//TODO: Mouse callbacks!
void Window::mouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Rotate = true;
		Move = false;
		lastPoint = trackBallMapping(x, y);
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		clickedX = x;
		clickedY = y;
		
		Rotate = false;
		Move = true;
		lastPoint = trackBallMapping(x, y);
	}

	else {
		Rotate = false;
		Move = false;
	}
}

//TODO: Mouse Motion callbacks!
void Window::motionCallback(int x, int y)
{
	Vector3 curPoint;
	Matrix4 trans;

	if (Rotate) {
		curPoint = trackBallMapping(x, y);

		Vector3 rotAxis = lastPoint.cross(curPoint);
		float rot_angle = lastPoint.angle(curPoint);

		trans.makeRotateArbitrary(rotAxis, rot_angle);

		if (object == 0) {
			moveObject(trans);
		}

		else if (object == 1) {
			Vector3 newPos = trans * Vector3(Globals::directional.position[0], Globals::directional.position[1], Globals::directional.position[2]);
			Globals::directional.position = Vector4(newPos[0], newPos[1], newPos[2], 0.0);
		}

		else if (object == 2) {
			Vector3 newPos = trans * Vector3(Globals::point.position[0], Globals::point.position[1], Globals::point.position[2]);
			Globals::point.position = Vector4(newPos[0], newPos[1], newPos[2], 1.0);
			pointLight.toWorld = trans * pointLight.toWorld;
		}

		else if (object == 3) {
			Vector3 newPos = trans * Vector3(Globals::spot.position[0], Globals::spot.position[1], Globals::spot.position[2]);
			Globals::spot.position = Vector4(newPos[0], newPos[1], newPos[2], 1.0);
			spotLight.toWorld = trans * spotLight.toWorld;
		}

		lastPoint = curPoint;
	}

	else if (Move) {
		if (object == 0) {
			curPoint = trackBallMapping(x, y);

			Vector3 curPoint2(curPoint[0], curPoint[1], 0.0);
			Vector3 lastPoint2(lastPoint[0], lastPoint[1], 0.0);

			Vector3 translate;
			translate = (curPoint2 - lastPoint2) * 10;

			trans.makeTranslate(translate);
			moveObject(trans);

			lastPoint = curPoint;
		}

		else if (object == 3) {
			std::cout << "Value of angle is " << Globals::spot.angle << std::endl;
			std::cout << "Value of exponent is " << Globals::spot.exponent << std::endl;

			if (spot == 1) {
				if (y > clickedY) {
					if (Globals::spot.angle + 0.5 <= 90) {
						Globals::spot.angle = Globals::spot.angle + 0.5f;
					}
				}

				else if (y < clickedY) {
					if (Globals::spot.angle - 0.5 >= 0) {
						Globals::spot.angle = Globals::spot.angle - 0.5f;
					}
				}
			}

			else {
				if (x > clickedX) {
					if (Globals::spot.exponent + 1.0 <= 128) {
						Globals::spot.exponent = Globals::spot.exponent + 1.0f;
					}
				}

				else if (x < clickedX) {
					if (Globals::spot.exponent - 1.0 >= 0) {
						Globals::spot.exponent = Globals::spot.exponent - 1.0f;
					}
				}
			}
		}
	}
}

void Window::mouseWheelCallback(int wheel, int direction, int x, int y)
{
	Matrix4 trans;
	Vector3 move;

	if (object == 0) {
		if (direction < 0) {
			trans.makeScale(1.1f);
		}

		else {
			trans.makeScale(0.9f);
		}

		scaleObject(trans);
	}

	else if (object == 2) {
		move = Globals::point.position.toVector3().normalize();

		if (direction < 0) {
			Globals::point.position = Globals::point.position + Vector4(move[0], move[1], move[2], 0.0);
			trans.makeTranslate(move);
			pointLight.toWorld = trans * pointLight.toWorld;
		}

		else {
			Globals::point.position = Globals::point.position - Vector4(move[0], move[1], move[2], 0.0);
			trans.makeTranslate(move.negate());
			pointLight.toWorld = trans * pointLight.toWorld;
		}
	}

	else if (object == 3) {
		move = Globals::spot.position.toVector3().normalize();

		if (direction < 0) {
			Globals::spot.position = Globals::spot.position + Vector4(move[0], move[1], move[2], 0.0);
			trans.makeTranslate(move);
			spotLight.toWorld = trans * spotLight.toWorld;
		}

		else {
			Globals::spot.position = Globals::spot.position - Vector4(move[0], move[1], move[2], 0.0);
			trans.makeTranslate(move.negate());
			spotLight.toWorld = trans * spotLight.toWorld;
		}
	}
}