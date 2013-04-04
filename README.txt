Victor Frenkel
Computer Graphics HW3

type 'make' to compile and run. NOTE: code MUST be run on clic-lab.cs.columbia.edu, not clic.cs.columbia.edu. The configurations are different.

CONTROLS: W,A,S,D for movement/banking. press and hold 'o' key to use the follow cam, let go for the throwing cam.

	  ***IMPORTANT***: after you are done viewing a shader, press its number key again to disable it. You must disable one shader to look at another. Program starts with no shaders enabled.
          Hit 1 for gouraud phong shader.
	  Hit 2 for blinn phong shader.
	  Hit 3 for an experiment with normal map shader (not quite done, not in tangent space so it looks weird).
	  Hit 4 for a vertex position based checkerboard shader.
	  Hit 5 for a proper uv based checkerboard shader (showing distortions in teapot uv mapping).

Sources/Credits:
	Looked through lighthouse3d tutorials on glsl and lighting, directional lights.
	Looked through nullterminator.net tutorial on opengl textures.

	Used SOIL library for loading a jpg image into a char array.

	Used turbosquid for the tron bike model, by japrogramer.
	Swiftless tutorials, opengl documentation, opengl wikibooks

	http://www.opengl.org/sdk/docs/man2/
	http://www.swiftless.com/opengltuts.html
	http://www.nullterminator.net/gltexture.html
	http://en.wikibooks.org/wiki/OpenGL_Programming


Organization/Overview:
	shaders directory has all the shaders used.
	GLSLProgram.cpp/h was modified from the starter code version to provide shader loading from file capability. Contains shader handling code.
	revolutejoint.cpp/h has the individual joint rendering and transformation code.
	skeleton.cpp/h has the ik solver and skeleton rendering code (the tree of joints).
	camera.cpp/h has camera motion control and transforms.
	gridpiece.ccp/h has the flat grid object, along with its display list definition.
	linalg.h has some handy geometry functions.
	main.cpp has the opengl initialization, some simple lighting code, initial setup.
	model.h has obj file loading code, model drawing code using vertex buffer objects.
	playercycler.cpp/h uses model.h to define and render the bike (transforms, materials, etc.). Has control logic for smooth controls. Houses the shader selection controls based on the sticky key pressed.
	scene.cpp/h stores all information about scene objects and state (like keyboard presses and display lists).

	Linked with the SOIL library to load images.
