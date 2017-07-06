#include <stdio.h>   
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "glut.h"  
#include "visuals.h"  

using namespace std;

// Model Loader
model md;

// Sun brightess
float transparency = 0.0f;
int grow = 0;

// Planet Rotations
float rotationAngle = 0.0f;

//Camera Movement
static float rotateX = 0.0;
static float rotateY = 0.0;

// Animation pause
static bool paused = false;

// Stars positions
float transparencyStars = 0.0f;
int growStars = 0;
float StarPos[20][3] =
{
	{20.0,   5.0,   3.0 }, {80.0,  32.0, -30.0}, { 21.0,  23.0,  18.0 }, {40.0, 10.0, -8.0}, {8.0, 12.0,  36.0},
	{13.0, -16.0, -12.0}, {67.0, -39.0, -34.0}, {17.0,  -45.0,  20.0}, {51.0,  -6.0,  -13.0}, {23.0,  -31.0,  9.0},
	{-20.0, 23.0, -25.0},{ -50.0, 32.0, 34.0 },{ -40.0,  7.0,  -17.0 },{ -65.0,  20.0,  7.0 },{ -31.0,  40.0,  15.0 },
	{-57.0, -18.0, -22.0},{ -50.0, -38.0, 19.0 },{ -65.0,  -3.0,  28.0 },{ -68.0, -38.0,  22.0 },{ -28.0, -25.0,  -5.0 }
};


void Render()
	{    
	if (!paused) 
		{
		//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
															 // and the depth buffer
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(0, 0, -100);
		glTranslatef(rotateX, rotateY, 0.0);
		

		stars(transparencyStars, StarPos);					// Set Stars
	
		solar_system(rotationAngle, transparency);			// Sun and Planets


		glutSwapBuffers();									// Buffer Swap									  
		}	
	}

//-----------------------------------------------------------

void Resize(int w, int h)
	{ 
	// define the visible area of the window ( in pixels )
	if (h == 0) 
		h = 1;
  
	glViewport(0, 0, w, h); 

	// Setup viewing volume

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
 
	gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
	}


void Idle()
	{
	if (!paused) 
		{
		// Planets Rotations
		rotationAngle += 0.15;

		// Sun brightness
		if (grow == 0)
			{
			transparency += 0.002;
			if (transparency < 1.0)
				grow = 0;
			else
				grow = 1;
			}
		else
			{
			transparency -= 0.002;
			if (transparency > 0.0)
				grow = 1;
			else
				grow = 0;
			}

		// Star brightness
		if (growStars == 0)
			{
			transparencyStars += 0.1;
			if (transparencyStars < 1.0)
				growStars = 0;
			else
				growStars = 1;
			}
		else
			{
			transparencyStars -= 0.001;
			if (transparencyStars > 0.0)
				growStars = 1;
			else
				growStars = 0;
			}

		glutPostRedisplay();
		}
	}

	
void Keyboard(unsigned char key,int x,int y)
	{
	switch(key)
		{
		case 'q' : 
			exit(0);
			break;
		case 'p': 
			paused = !paused;
			break;
		case 'a' : 
			rotateX += 0.5f;
			break;
		case 'd' : 
			rotateX -= 0.5f;
			break;
		case 'w':  
			rotateY -= 0.5f;
			break;
		case 's':  
			rotateY += 0.5f;
			break;
		default : 
			break;
		}
	glutPostRedisplay();
	}


void Setup()  
	{ 
	ReadFile(&md);

	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	
	glEnable(GL_BLEND);		// incoming //  // stored //
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	}



void ReadFile(model *md)
	{
	ifstream obj_file("planet.obj");								// Open the file for reading planet.TXT  

	string tmp;
	int counter = 0;

	md->vertices = 9122;											// Get the number of vertices
	md->faces = 18240;												// Get the number of faces

	if (obj_file.fail()) 
		{
		cout << "Failed to open the file" << endl;
		exit(1);
		}

	obj_file >> tmp;

	while (tmp.compare("v"))									
		obj_file >> tmp;


	for (int i = 0; i < md->vertices ; i++)							// Get the vertex coordinates
		{              
		obj_file >> md->obj_points[i].x;
		obj_file >> md->obj_points[i].y;
		obj_file >> md->obj_points[i].z;
		
		obj_file >> tmp;
		}

	while (tmp.compare("f")) 
		obj_file >> tmp;


	for (int i = 0; i < md->faces; i++)									// Get the face structure  
		{
		obj_file >> md->obj_faces[i].vtx[0];

		obj_file >> tmp;

		obj_file >> md->obj_faces[i].vtx[1];

		obj_file >> tmp;

		obj_file >> md->obj_faces[i].vtx[2];

		if (i != (md->faces - 1))
			{
			while (tmp.compare("f"))
				obj_file >> tmp;
			}
		}
		
	obj_file.close();
	}


void DisplayModel(model md)
	{
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
		{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
		}

	glEnd();
	glPopMatrix();
	}



void solar_system(float rotationAngle, float transparency)
	{
	// Create Sun
	glColor4f(1.0, 0.9, 0.0, 1.0);								// Set yellow drawing colour with stable transparency
	glutSolidSphere(12.0, 30, 24);

	//Create sun brightness
	glPushMatrix();
	glTranslatef(0.0, 0.0, 12.0);
	glColor4f(1.0, 0.9, 0.0, transparency);						// Set yellow drawing colour with changing transparency
	glutSolidSphere(15.0, 30, 24);
	glPopMatrix();



	// Create planet 1 - Bottom
	glPushMatrix();
	glScalef(0.01, 0.01, 0.01);

	glRotatef(rotationAngle, 1.0f, 0.0f, 0.0f);					// Rotate around Sun
	glTranslatef(0, 4000, 0);									
	glRotatef(5 * rotationAngle, 1.0f, 0.0f, 0.0f);				// Rotate around itself

	glColor3f(0.0, 0.5, 0.0);									// Set drawing colour   
	DisplayModel(md);
	glPopMatrix();

	// Create planet 2 - Top
	glPushMatrix();
	glScalef(0.015, 0.015, 0.015);

	glRotatef(rotationAngle, 1.0f, 0.0f, 0.0f);					// Rotate around Sun
	glTranslatef(0, -3000, 0);
	glRotatef(5 * rotationAngle, 1.0f, 0.0f, 0.0f);				// Rotate around itself

	glColor3f(0.0, 1.0, 1.0);									// Set drawing colour 
	DisplayModel(md);
	glPopMatrix();



	// Create planet 3 - Left
	glPushMatrix();
	glScalef(0.011, 0.011, 0.011);

	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);					// Rotate around Sun
	glTranslatef(4000, 0, 0);
	glRotatef(5 * rotationAngle, 0.0f, 1.0f, 0.0f);				// Rotate around itself

	glColor3f(1.0, 0.5, 0.9);									// Set drawing colour 
	DisplayModel(md);
	glPopMatrix();

	// Create planet 4 - Right
	glPushMatrix();
	glScalef(0.008, 0.008, 0.008);

	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);					// Rotate around Sun
	glTranslatef(-4500, 0, 0);
	glRotatef(5*rotationAngle, 0.0f, 1.0f, 0.0f);				// Rotate around itself

	glColor3f(0.3, 0.2, 0.9);									// Set drawing colour
	DisplayModel(md);
	glPopMatrix();
	}


void stars(float transparencyStars, float position[][3])
	{
	for (int i = 0; i < 20; i++)
		{
		// Create Star
		glPushMatrix();
		glTranslatef(position[i][0], position[i][1], position[i][2]);			// -100 < x < 100, -50 < y < 50, -50 < z < 50
		glColor4f(1.0, 1.0, 1.0, 1.0);											// Set white drawing colour with stable transparency
		glutSolidSphere(0.2, 30, 24);
		glPopMatrix();

		// Create Brightness
		glPushMatrix();
		glTranslatef(position[i][0], position[i][1], position[i][2]);
		glColor4f(1.0, 1.0, 1.0, transparencyStars);                            // Set white drawing colour with changing transparency
		glutSolidSphere(0.3, 30, 24);
		glPopMatrix();
		}
	}