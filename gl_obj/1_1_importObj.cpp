/*
 *  1_1_importObj.cpp
 *  gl
 *
 *  Created by Haru (HyunKyung) Ji on 1/25/11.
 *  Copyright 2011 UCSB. All rights reserved.
 *
 *  Quesiton: how to calculate the whole lighting effect, relative?
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include "main.h"
#include <string.h>

float rotX1 = 0;
//position the light
float pos[] = {.0, .0, 0.0, 1.0}; //if the last value 0 = positional, otherwise directional

int NUM_SIDES = 3;
float SCALE_VERTICES = .3; //1.0

float rotY = 0.0;
float transZ = -5.0;
int numVertices, numNormals, numTexCoords, numFaces;
struct vec3 *vertices, *normals, *texcoords;
struct face *faces;

void print_vec3(struct vec3 p) {
	printf("%f/%f/%f", p.x, p.y, p.z);
}

void print_faces() {
	printf("\n");
	int i;
	for (i = 0; i < numFaces; i++) {
		printf("face #%d:\n", i); print_face(faces[i]);
	}
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

void print_face(struct face f) {
	printf("\npt 1: v:"); print_vec3(vertices[f.v1]);
	printf(" t:"); print_vec3(texcoords[f.t1]);
	printf(" n:"); print_vec3(normals[f.n1]);
	printf("\npt 2: v:"); print_vec3(vertices[f.v2]);
	printf(" t:"); print_vec3(texcoords[f.t2]);
	printf(" n:"); print_vec3(normals[f.n2]);
	printf("\npt 3: v:"); print_vec3(vertices[f.v3]);
	printf(" t:"); print_vec3(texcoords[f.t3]);
	printf(" n:"); print_vec3(normals[f.n3]);
	printf("\n");
}

void scanVertex(char line[], float scale) {
	numVertices++;
	vertices = (struct vec3 *)realloc (vertices, sizeof(struct vec3) * numVertices);
	float a, b, c;
	sscanf (line, "v %f %f %f", &a, &b, &c);
	struct vec3 v;
	v.x = a * scale; v.y = b * scale; v.z = c * scale;
	vertices[numVertices - 1] = v; 
}

void scanTexCoord(char line[]) {
	numTexCoords++;
	texcoords = (struct vec3 *)realloc (texcoords, sizeof(struct vec3) * numTexCoords);
	float a, b;
	sscanf (line, "vt %f %f", &a, &b);
	struct vec3 tc;
	tc.x = a; tc.y = b; tc.z = 0.0;
	texcoords[numTexCoords - 1] = tc; 
}

void scanNormal(char line[]) {
	numNormals++;
	normals = (struct vec3 *)realloc (normals, sizeof(struct vec3) * numNormals);
	float a, b, c;
	sscanf (line, "vn %f %f %f", &a, &b, &c);
	struct vec3 n;
	n.x = a; n.y = b; n.z = c;
	normals[numNormals - 1] = n; 
}

void scanFace(char line[], int numSides) {
	numFaces++;
	faces = (struct face *) realloc(faces, sizeof(struct face) * numFaces);
	int v1, t1, n1, v2, t2, n2, v3, t3, n3, v4, t4, n4;
	if (numSides == 3) {
		sscanf (line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
	} else {
		sscanf (line, "f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3, &v4, &t4, &n4);
	}
	struct face f;
	f.v1 = v1 - 1; f.v2 = v2 - 1; f.v3 = v3 - 1; f.v4 = v4 - 1; 
	f.t1 = t1 - 1; f.t2 = t2 - 1; f.t3 = t3 - 1; f.t4 = t4 - 1;
	f.n1 = n1 - 1; f.n2 = n2 - 1; f.n3 = n3 - 1; f.n4 = n4 - 1;
	f.numSides = numSides;
	faces[numFaces - 1] = f; 
}

void readFile(char* filename) {
	vertices = (struct vec3 *) malloc (sizeof(struct vec3));
	normals = (struct vec3 *) malloc (sizeof(struct vec3));
	texcoords = (struct vec3 *) malloc (sizeof(struct vec3));
	faces = (struct face *) malloc (sizeof(struct face));
	
	FILE *file = fopen ( filename, "r" );
	if ( file != NULL )
	{
		char line[128]; 
		
		while(fgets(line, sizeof line, file) != NULL) {
			if (strncmp( line, "v ", 2) == 0) {
				scanVertex(line, SCALE_VERTICES);
			} else if (strncmp( line, "vt", 2) == 0) {
				scanTexCoord(line);
			} else if (strncmp( line, "vn", 2) == 0) {
				scanNormal(line);
			} else if (strncmp( line, "f ", 2) == 0) {
				scanFace(line, NUM_SIDES);
			}
		}
		fclose(file);
	}
	else {
		perror(filename);
	}
}

void displayFaces() {
	
	int i;
	for (i = 0; i < numFaces; i++) {
		struct face f = faces[i];
		
		glBegin(GL_TRIANGLES);
		
		//v1
		struct vec3 n1 = normals[f.n1]; 
		struct vec3 t1 = texcoords[f.t1]; 
		struct vec3 p1 = vertices[f.v1]; 
		
		glNormal3f(n1.x, n1.y, n1.z);
		glTexCoord2f(t1.x, t1.y);  
		glVertex3f(p1.x, p1.y, p1.z);  
		
		//v2
		struct vec3 n2 = normals[f.n2]; 
		struct vec3 t2 = texcoords[f.t2]; 
		struct vec3 p2 = vertices[f.v2]; 
		
		glNormal3f(n2.x, n2.y, n2.z);
		glTexCoord2f(t2.x, t2.y);
		glVertex3f(p2.x, p2.y, p2.z); 
		
		//v3
		struct vec3 n3 = normals[f.n3];
		struct vec3 t3 = texcoords[f.t3]; 
		struct vec3 p3 = vertices[f.v3]; 
		
		glNormal3f(n3.x, n3.y, n3.z);
		glTexCoord2f(t3.x, t3.y);
		glVertex3f(p3.x, p3.y, p3.z);
		
		glEnd();  
	}
}

void myInit(void)
{	
	glDepthFunc(GL_LEQUAL); 
	//glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0,0.0,0.0,0.0);
	readFile("/Users/jiharu/svn/594CM/gl_obj/obj/triangular/Shatter1.obj");
	
	//DEFINE NIGTHS in some INIT function..
	glEnable(GL_LIGHTING); //enable lighting
	
	glShadeModel (GL_SMOOTH); //GL_SMOOTH, GL_FLAT
	
	//define the lighting model
	float ambient[] = {0.08, 0.08, 0.01, 1.0};
	float diffuse[] = {0.1, 0.1, 0.1, 1.0};
	float specular[] = {0.3, 0.3, 0.3, 1.0};
	float emission[] = {.01, .02, .02, 1.0};
	
	glEnable(GL_LIGHT0); //turn on one of the lights
	//	glDisable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_EMISSION, emission);
	
	//spot light 1
	float spot_direction1[] = { 0.0, -1.0, 0.0 }; 
	
	float ambient1[] = {0.01, .2, 0.2, 1.0};
	float diffuse1[] = {0.01, 0.5, 0.5, 1.0};
	float specular1[] = {.0, 0.3, .3, 1.0};
	float emission1[] = {.0, 0.0, .1, 1.0};
	
	glEnable(GL_LIGHT1); //turn on one of the lights
	//	glDisable(GL_LIGHT1);	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);	
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1, GL_EMISSION, emission1);	
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5); 
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5); 
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);	// degrees
	glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	
	//spot light 2
	float spot_direction2[] = { 0.0, -1.0, 0.0 }; 
	
	float ambient2[] = {0.3, 0.0, 0.01, 1.0};
	float diffuse2[] = {0.5, 0.0, 0.01, 1.0};
	float specular2[] = {3.0, 0.0, 0.0, 1.0};
	float emission2[] = {.1, 0.0, 0.0, 1.0};
	
	glEnable(GL_LIGHT2); //turn on one of the lights
	//	glDisable(GL_LIGHT2);	
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);	
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_EMISSION, emission2);
	
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5); 
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.8); 
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.8);
	glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);	// degrees
	glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);

	float spot_direction3[] = { .5, -1.0, 0.0 }; 
	float ambient3[] = {0.1, 0.2, 0.4, 1.0};
	float diffuse3[] = {1.0, 0.7, 0.3, 1.0};
	float specular3[] = {1.0, 0.3, 0.0, 1.0};
	
	glEnable(GL_LIGHT3); //turn on one of the lights
	//	glDisable(GL_LIGHT3);	
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);	
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular3);
	
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction3);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5); 
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.8); 
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf (GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);	// degrees
	glLightf (GL_LIGHT3, GL_SPOT_EXPONENT, 2.0);
	
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

void display(void)
{	
	//define the material of the object
	float no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	float mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_gray1[] = { 0.3, 0.3, 0.3, 1.0 };
	float mat_gray2[] = { 0.5, 0.5, 0.5, 1.0 };
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
	
	glTranslatef(0.0, 0.0, -6.0);    //set origin of shape (5 units into the distance)
	glPushMatrix(); 
	glTranslatef(-sin(0.5), 1.2, 2.0);
	glLightfv(GL_LIGHT3, GL_POSITION, pos); //spot light3
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_gray1);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_red); 
	glutSolidCube(0.11);
	glPopMatrix(); 

	glPushMatrix(); 
	glTranslatef(0.0, -0.5, 2.0);
	glRotatef(rotX1, 0.0, 1.0, 0.0); 
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_gray2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white); 
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_gray2);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
	displayFaces();	
	glPopMatrix(); 
		
	glPushMatrix(); 
	glTranslatef(-2.0, 0.0, -2.0); 
	glLightfv(GL_LIGHT1, GL_POSITION, pos); //spot light1
	glRotatef(rotX1, 1.0, 0.0, 0.0); 
	//glutSolidSphere(0.8, 10, 10);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_green);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
	glutSolidSphere(0.45, 10, 10);
	//glutSolidCube(.5);	
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
	glLightfv(GL_LIGHT2, GL_POSITION, pos); //spot light2
	glRotatef(-rotX1, 1.0, 0.0, 0.0); 
	//	glutSolidTorus(0.3, 0.8, 32, 32);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_orange);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 
	glutSolidSphere(0.45, 10, 10);
	//glutSolidCube(.5);	
	glPopMatrix(); 
	
	glPushMatrix(); 		
	glTranslatef(2.0, 0.0, -2.0); 
	glRotatef(-rotX1, .0, .0, 1.0); 
	glTranslatef(.0, 1.0, .0);
	glRotatef(-rotX1, .0, 1.0, .0); 
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_purple);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_blue);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat); 	
	glutSolidCube(.2);	
	glPopMatrix(); 
	
	glPushMatrix(); 
//	glTranslatef(-3.0, -3, -2.0); 
//	glRotatef(rotX1, 1.0, 0.0, 0.0); 
	glTranslatef(.0, 1, .0); 
	glRotatef(rotX1, .0, 0.0, 1.0); 
	glTranslatef(2.5, .1, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos); // sun light
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_orange); 
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_bright); 
	glutSolidSphere(0.15, 10, 10);	 // Sun
	glPopMatrix();            //end matrix
 	
	//printNormal(1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_red); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_shininess); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_purple);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat); 
	
	for (int z = -14; z <= 14; z ++){		
		
		glBegin(GL_QUAD_STRIP);
		for (int x = -8; x <= 8; x ++){

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
			
			glVertex3d(x, sin(z+1) * (sin(3.3*x))-2, z+1);
			glVertex3d(x, sin(z) * (sin(3.3*x))-2, z);		
		}
		glEnd();

	}	
	
	glutSwapBuffers();
	
	rotX1 += .25;	
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);    //
	glMatrixMode(GL_PROJECTION);        //
	glLoadIdentity();                    //
	gluPerspective(45.0, (float)width/(float)height, .1, 100.0); //far plane is set to 100 units
}

void keyboard(unsigned char key, int x, int y) {
	printf("key pressed %c (%d) \n", key, key);
	
	switch(key) {
		case 'a':
			break;
			
		case 'b':
			break;
			
		case 'q': 
		case 27: //Escape key
			exit(0); 
	}
}

void keySpecial(int key, int x, int y) {  
	printf("special key pressed:< %c > \n", key);
	
	switch(key) {
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_UP:
			transZ += 0.1;
			break;
		case GLUT_KEY_DOWN:
			transZ -= 0.1;
			break;
		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;
		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
	}
}

void timer(int value) {
	rotY += 1.0;
	glutTimerFunc(1000/60,timer,0);
}

void mouseMoved(int x, int y) {
	printf("mouse at %i/%i\n", x, y);
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
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMoved);
	glutSpecialFunc(keySpecial);
	glutTimerFunc(1000/60,timer,0);
	
	myInit();
	
	glutMainLoop();
	return EXIT_SUCCESS;
}	
