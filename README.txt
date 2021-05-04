The built .exe versions of both the debug and release versions of the program are included
They can be found at (ProjectDirectory)->bin->x64->Release/Debug->OpenGL.exe
The Release version is recommended as while both versions are visually the same, the Release version has superior performance and loading times

The scene shows most of the features originally promised in my project proposal and allows the user to fly around to see them all for themselves

Controls:
W/A/S/D to move around the camera
Moving the mouse moves the front facing vector of the camera
1/2/3/4/5 changes the screen filter applied to the drawn frame
	1 Normal 
	2 Inverse
	3 Greyscale
	4 Edge detection
	5 ???
6 toggles the scenes directional light on and off
7 toggles the scenes spotlight on and off - this light is off by default and is attached to the camera, acting as a sort of torch
8/9/0/- toggles the scenes point lights on and off
	8 is the point light in the starting house
	9 is the point light moving along the X and Z axis showing the different materials on the floor
	0 is the point light moving along the Z axis showing the difference between using a normal map and no normal map
	- is the colour changing light showcasing coloured lighting

