/*
 *  01_rectangle.c
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

void display(void)
{
	//glPushMatrix();            //start matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);    //set color of rectangle
		
	glTranslatef(0.0, 0.0, -8.0);    //set origin of shape (5 units into the distance)

	glPushMatrix(); 
		glTranslatef(-2.0, 0.0, 0.0); 
		glRotatef(rotX1, 1.0, 1.0, 1.0); 
		glutWireCube(.5);
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		glTranslatef(.0, 1.0, .0);	
		glutWireCube(.1);		
	glPopMatrix(); 
	
	glPushMatrix(); 
		glTranslatef(2.0, 0.0, 0.0); 
		glColor3f(0.3, 0.3, 0.3); 
		glutWireCube(.5);
		glRotatef(rotX1, 0.0, 1.0, 0.0); 
		glTranslatef(.0, .0, 1.0);
		glRotatef(rotX1, 0.0, 1.0, 1.0); 		
		glutWireCube(.2);		
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
	
	
	glutMainLoop();
	return EXIT_SUCCESS;
}

