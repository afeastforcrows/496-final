//////////////////////////////////////////////////////////////////
//	Walter Wyatt Dorn					//
//	CPSC 486						//
//	Camera Control & View Frustum Culling – Assignment 3	//
//	Dr. Shafae - CSU Fullerton				//	
//	5/18/16							//
//////////////////////////////////////////////////////////////////

README:

Controls for the program are as follows:
	shift+mouse: trackball movement
	b: toggle rendering the bounding volumes
	i: print a help message
	q/esc: quit
	r: reset camera orientation to default state (bugged)
	w, a, s, d: rotate the selected model (bugged)
	arrow keys: moves the camera forward, backward, to the left and to the right
	t, f, g, h: translate the selected model

Known bugs:
	-The 'R' key briefly resets the camera viwe, but the change is not permanent and will revert when the camera is moved again
	-When rotating individual models, they rotate about an axis that does not cross their center
	-If the camera enters a model or bounding sphere, the entire scene becomes wireframe

Ply models used in this project were taken from:
http://people.sc.fsu.edu/~jburkardt/data/ply/

To compile the program, tyep the following in to the terminal:
$ make

To run the program, tyep the following in to the terminal:
$ ./vfculling
