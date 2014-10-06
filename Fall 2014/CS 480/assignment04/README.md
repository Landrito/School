PA4 Model Loading
===========================================

General Information
-------------------

This is a program used to show the understanding of the basic concepts of 
OpenGL, Glut and Glew. The program shows a .obj model loaded with a model loader.


Loading the Model
-----------------
To load the model, all you have to do is pass the .obj file path as an argument to the
executable at runtime.

ex: > ./Matrix dragon.obj

Interactions
------------

Menu: To see a menu of options, right click inside the window.
	Start Cube Spin: this will make the cube spin about its y axis
	Stop Cube Spin: this will stop the cube from spinning
	Exit: this will exit the program

Keyboard:
	a or A: Reverse the direction the cube is rotating
	s or S: Increase the speed the cube is rotating clockwise
	d or D: Decrease the speed the cube is rotating clockwise

Mouse:
	left-click: Reverse the direction the cube is rotating
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
