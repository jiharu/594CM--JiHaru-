/*
 *  03_crossproduct.cpp
 *  gl
 *
 *  Created by Haru (HyunKyung) Ji on 1/12/11.
 *  Copyright 2011 UCSB. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>

struct vec3 {
	float x, y, z;	
	
	float length (){
		return sqrt((x*x) + (y*y) + (z*z));
	}
	void normalize (){
		float l = length();
		x = x / l ;
		y = y / l ;
		z = z / l ;
	}
};



float rotX1 = 0;
//position the light
float pos[] = {.0, .0, 0.0, 1.0}; //if the last value 0 = positional, otherwise directional

void myInit(void)
{	
	glDepthFunc(GL_LEQUAL); 
	//glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	//DEFINE NIGTHS in some INIT function..
	glEnable(GL_LIGHTING); //enable lighting
	
	glShadeModel (GL_SMOOTH); //GL_SMOOTH, GL_FLAT
		
	//define the lighting model
	float ambient[] = {0.3, 0.3, 0.3, 1.0};
	float diffuse[] = {0.8, 0.8, 0.0, 1.0};
	float specular[] = {1.0, 1.0, 1.0, 1.0};
	float emission[] = {.0, .0, .0, 1.0};
	
	glEnable(GL_LIGHT0); //turn on one of the lights
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_EMISSION, emission);
		
	//define the material of the object
	float mat_shininess[] = { 50. };
	float mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	float mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_emission[] = {1.0, 0.1, 0.1, 0.0};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission); 
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);	
}

/*void printNormal(float x1, float y1, float z1, float x2, float y2, float z2) {					
	float nx = (y1 * z2) - (y2 * z1);
	float ny = (z1 * x2) - (z2 * x1);
	float nz = (x1 * y2) - (x2 * y1);
	printf("the normal is...??? %f %f %f \n %f %f %f", nx, ny, nz); 
}*/

vec3 makeNormal(vec3 v1, vec3 v2){
	vec3 norm;
	norm.x = (v1.y * v2.z) - (v2.y * v1.z);
	norm.y = (v1.z * v2.x) - (v2.z * v1.x);
	norm.z = (v1.x * v2.y) - (v2.x * v1.y);
	norm.normalize(); // call the method
	return norm;
}

vec3 calulateVector(vec3 p0, vec3 p1, vec3 p2) {
	vec3 v1; vec3 v2;
	v1.x = (p1.x - p0.x); 
	v1.y = (p1.y - p0.y);
	v1.z = (p1.z - p0.z);
	v2.x = (p2.x - p0.x);
	v2.y = (p2.y - p0.y);
	v2.z = (p2.z - p0.z);
	return makeNormal(v1, v2);	
}
	
void display(void)
{	
	//define the material of the object
	float no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	float mat_red[] = { 1.0, 0.2, 0.1, 1.0 };
	float mat_orange[] = { 0.7, 0.5, 0.1, 1.0 };
	float mat_yellow[] = { 0.8, 0.8, 0.1, 1.0 };
	float mat_green[] = { 0.0, 1.0, 0.0, 1.0 };
	float mat_blue[] = { 0.1, 0.1, 0.8, 1.0 };
	float mat_purple[] = { 0.8, .1, 0.8, 1.0 };
	float no_shininess[] = { 0.0 };
	float low_shininess[] = { 5.0 };
	float high_shininess[] = { 100.0 };
	float mat_bright[] = {1.0, 0.7, 0.1, 1.0};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);    //set color of rectangle
	
	glTranslatef(0.0, 0.0, -8.0);    //set origin of shape (5 units into the distance)

	glPushMatrix(); 
		glTranslatef(-2.0, 0.0, -2.0); 
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		//glutSolidSphere(0.8, 10, 10);
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_orange);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
		glutSolidCube(.5);	
		glRotatef(rotX1, 1.0, 0.0, 0.0); 
		glTranslatef(.0, 1.0, .0);
		//glutWireCube(.1);	   
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_orange);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_green); 
			glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_blue);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
		glutSolidCube(.1);	
	glPopMatrix(); 
	
	glPushMatrix(); 
		glTranslatef(2.0, 0.0, -2.0); 
		glRotatef(-rotX1, 1.0, 0.0, 0.0); 
	//	glutSolidTorus(0.3, 0.8, 32, 32);
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_purple);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_blue);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
		glutSolidCube(.5);	
	glPopMatrix(); 
	
	glPushMatrix(); 		
		glTranslatef(2.0, 0.0, -2.0); 
		glRotatef(-rotX1, .0, .0, 1.0); 
		glTranslatef(.0, 1.0, .0);
		glRotatef(-rotX1, .0, 1.0, .0); 
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_red); 
			glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
		glutSolidCube(.2);	
	glPopMatrix(); 
	
	glPushMatrix(); 
		glTranslatef(-3.0, -3, -2.0); 
		glRotatef(rotX1, 1.0, 1.0, 0.0); 
		glTranslatef(0.0, 7.1, 0.0);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
				glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
				glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red);
				glMaterialfv(GL_FRONT, GL_EMISSION, mat_bright); 
			glutSolidSphere(0.15, 10, 10);	 
	glPopMatrix();            //end matrix
 	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_red); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_purple);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat); 
	
	for (int z = -8; z <= 0; z ++){	
		glBegin(GL_QUADS);
		for (int x = -4; x <= 4; x ++){
			vec3 p0; vec3 p1; vec3 p2; 
			p0.x = x;
			p0.y = -2;
			p0.z = z;		
			p1.x = x;
			p1.y = -2;
			p1.z = z + 1;
			p2.x = x + 1;
			p2.y = -2;
			p2.z = z;
			vec3 norm = calulateVector(p0, p1, p2);
			glNormal3d(norm.x, norm.y, norm.z);
			
			glVertex3d(x, -2, z);	
			glVertex3d(x, -2, z + 1);	
			glVertex3d(x + 1, -2, z + 1);
			glVertex3d(x + 1, -2, z);		
		}
		glEnd();
	}	

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

