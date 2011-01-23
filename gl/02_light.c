/*
 *  02_light.c
 *  gl
 *
 *  Created by Haru (HyunKyung) Ji on 1/12/11.
 *  Copyright 2011 UCSB. All rights reserved.
 *
 */

#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>

float rotX1 = 0;
//position the light
float pos[] = {.0, .0, 0.0, 1.0}; //if the last value 0 = positional, otherwise directional

void myInit(void)
{
	
	//glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	//DEFINE NIGTHS in some INIT function..
	glEnable(GL_LIGHTING); //enable lighting
	glEnable(GL_LIGHT0); //turn on one of the lights
		
	//define the lighting model
	float ambient[] = {1.0, 0.0, 0.0, 1.0};
	float diffuse[] = {0.0, 1.0, 0.0, 1.0};
	float specular[] = {1.0, 1.0, 1.0, 1.0};	

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);	
	
	//define the material of the object
	float objHighlights[] = {1.0, 1.0, 1.0, 1,0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, objHighlights);
	float specularExponent = 128;
	glMaterialf(GL_FRONT, GL_SHININESS, specularExponent);
	
}

void display(void)
{
	//glPushMatrix();            //start matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);    //set color of rectangle
	
	
	glTranslatef(0.0, 0.0, -5.0);    //set origin of shape (5 units into the distance)
	//glRotatef(20.0, 1.0, 0.0, 0.0); //rotate 20 degrees around the x axis
	
	glTranslatef(0.0, 0.0, -5.0); 
	glPushMatrix(); 
		glTranslatef(-2.0, 0.0, 0.0); 
		glRotatef(rotX1, 1.0, 1.0, 1.0); 
		glutSolidSphere(0.8, 10, 10);
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		glTranslatef(.0, 1.0, .0);
		//glLightfv(GL_LIGHT0, GL_POSITION, pos);	
		glutWireCube(.1);		
	glPopMatrix(); 
	
	glPushMatrix(); 
		glTranslatef(2.0, 0.0, 0.0); 
		glColor3f(0.3, 0.3, 0.3); 
		glutSolidTorus(0.3, 0.8, 32, 32);
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		glTranslatef(.0, 2.0, .0);
		glRotatef(rotX1, 0.0, 0.0, 0.0); 
		glutSolidSphere(0.2, 10, 10);		
	glPopMatrix(); 
	
	glPushMatrix(); 
		glTranslatef(0.0, 0.0, 0.0); 
		glColor3f(0.3, 0.3, 0.3); 
		glRotatef(rotX1, 0.0, 1.0, 0.0); 
		glTranslatef(4.0, .0, .0);
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glutSolidSphere(0.15, 10, 10);		
	glPopMatrix();            //end matrix
	
	glutSwapBuffers();
	
	rotX1 += .1;
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);    //
	glMatrixMode(GL_PROJECTION);        //
	glLoadIdentity();                    //
	gluPerspective(45.0, (float)width/(float)height, .1, 100.0); //far plane is set to 100 units
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char** argv) //THIS IS THE BARE MINIMUM OF A GLUT PROJECT
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	
	glutCreateWindow("GLUT Program");
	
	glutDisplayFunc(display); //REGISTERS CALLBACK FUNCTIONS
	glutReshapeFunc(reshape); //RESIZING/VIEWPORT
	glutIdleFunc(idle);
	glClearColor(0.0,0.0,0.0,0.0);
	
	myInit();
	
	glutMainLoop();
	return EXIT_SUCCESS;
}

