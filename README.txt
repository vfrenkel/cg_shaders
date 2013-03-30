Victor Frenkel
Computer Graphics HW3

type 'make' to compile and run. NOTE: code MUST be run on clic-lab.cs.columbia.edu, not clic.cs.columbia.edu. The configurations are different.

BIKE CONTROLS: W,A,S,D for movement/banking. press and hold 'o' key to use the follow cam, let go for the throwing cam.

Sources/Credits:
	Looked through lighthouse3d tutorials on glsl and lighting, directional lights.

	Used turbosquid for the tron bike model, by japrogramer.
	Swiftless tutorials, opengl documentation, opengl wikibooks

	http://www.opengl.org/sdk/docs/man2/
	http://www.swiftless.com/opengltuts.html
	http://en.wikibooks.org/wiki/OpenGL_Programming


Organization/Overview:
	GLSLProgram.cpp/h was modified from the starter code version to provide shader loading from file capability. Contains shader handling code.
	revolutejoint.cpp/h has the individual joint rendering and transformation code.
	skeleton.cpp/h has the ik solver and skeleton rendering code (the tree of joints).
	camera.cpp/h has camera motion control and transforms.
	gridpiece.ccp/h has the flat grid object, along with its display list definition.
	linalg.h has some handy geometry functions.
	main.cpp has the opengl initialization, some simple lighting code, initial setup.
	model.h has obj file loading code, model drawing code using vertex buffer objects.
	playercycler.cpp/h uses model.h to define and render the bike (transforms, materials, etc.). Has control logic for smooth controls.
	scene.cpp/h stores all information about scene objects and state (like keyboard presses and display lists).
