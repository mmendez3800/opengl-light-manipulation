# OpenGL 3D Scenes
This project implements basic light manipulation using OpenGL

## General Info
When the user starts the project, they will be able to perform the below operations through the OpenGL window:
 * r - reset 3D scene
 * s - switch the angle of the spotlight
 * f - show frames-per-second (FPS)
 * 0 - focus is placed on object
 * 1 - focus is placed on
 * 2 - focus is placed on
 * 3 - focus is placed on
 * F1 - display sphere scene
 * F2 - display whale object
 * F3 - display ship object
 * Left Mouse Button - rotate object
 * Right Mouse Button - move object
 * Scroll Wheel - zoom in/out of object

## Technologies
This project was run using the following:
* Visual Studio 2022
* FreeGlut 3.0.0 MSVC

## Setup
After cloning or forking the repository, you can run the project in the below manner:
1. Download freeglut 3.0.0 for MSVC
2. Extract the contents of the zip file and place it in the C drive
   - This should result in `C:\freeglut` existing
3. Go to `C:\freeglut\bin`
4. Move `freeglut.dll` to `C:\Windows\System32` or `C:\Windows\SysWOW64` (depending on your operating system)
5. Double click file `Cube.sln` to open Visual Studio
6. Go to the Solution Explorer window and right-click **Cube** (not **Solution 'Cube'**)
7. Select **Properties** and left-click **C/C++**
8. At **Additional Include Directories**, add `C:\freeglut\include`
9. Left-click **Linker**
10. At **Additional Library Directories** add `C:\freeglut\lib`

## Execution
If seup is completed successfully, simply build the project by either clicking the green **Start** button
* You can also press **F5** or **Ctrl + F5**
