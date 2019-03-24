#include <stdio.h>   
#include "glut.h"   
                   
#include "visuals.h"   


int main(int argc, char* argv[]) { 
	// initialize GLUT library state
	glutInit(&argc, argv);
		
	// Set up the display using the GLUT functions to 
	// get rid of the window setup details:
	// - Use true RGB colour mode ( and transparency )
	// - Enable double buffering for faster window update
	// - Allocate a Depth-Buffer in the system memory or 
	//   in the video memory if 3D acceleration available	
						//RGBA//DEPTH BUFFER//DOUBLE BUFFER//
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	 
	  
	// Define the main window size and initial position 
	// ( upper left corner, boundaries included )
	glutInitWindowSize(700,700);
	glutInitWindowPosition(600,200);
	  
	// Create and label the main window
	glutCreateWindow("Solar System 2016");
	  
	// Configure various properties of the OpenGL rendering context
	Setup();
	  
	// Callbacks for the GL and GLUT events:

	// The rendering function 
	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);

	glutIdleFunc(Idle);

	glutKeyboardFunc(Keyboard);

	//Enter main event handling loop
	glutMainLoop();
	
	return 0;	
	}  
