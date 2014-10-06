PA3 Moons
===========================================

General Information
-------------------

This is a program used to show the understanding of the basic concepts of 
OpenGL, Glut and Glew. The program shows a color cube rotating about a center 
point while spinning as well as a smaller cube emulating a moon rotating about the cube.
Rotation and spinning are attributes of the cube that can be directly changed through the interactions in the code.a

Interactions
------------

Menu: To see a menu of options, right click inside the window.
	Start Cube Spin: this will make the cube spin about its y axis
	Stop Cube Spin: this will stop the cube from spinning
	Exit: this will exit the program

Keyboard:
	a or A: 			Reverse the direction the center cube is spinning
	s or S: 			Increase the speed the moon is rotating about the center cube
	d or D: 			Decrease the speed the moon is rotating about the center cube
	left arrow key:		Make the center cube orbit counter-clockwise about the center
	right arrow key:	Make the center cube orbit clockwise about the center

Mouse:
	left-click: Reverse the direction the cube is spinning
	right-click: see a menu of options







Building This Example
---------------------

*This example requires GLM*
*On ubuntu it can be installed with this command*

>$ sudo apt-get install libglm-dev

*On a Mac you can install GLM with this command(using homebrew)*
>$ brew install glm

To build this example just 

>$ cd build
>$ make

*If you are using a Mac you will need to edit the makefile in the build directory*

The excutable will be put in bin

Additional Notes For OSX Users
------------------------------
Ensure that the latest version of the Developer Tools is installed.
