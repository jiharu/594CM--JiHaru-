
#include <stdio.h>
#include <math.h>

struct face {
  int numSides;
  int v1, t1, n1, v2, t2, n2, v3, t3, n3, v4, t4, n4;
};

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

void keyboard(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void idle(void);
void display(void);
void reshape(int width, int height);
void timer(int value);
void print_face(struct face f);
void print_faces(void);
void print_p3f(struct p3f p);
void scanVertex(char line[], float scale);
void scanNormal(char line[]);
void scanTexCoord(char line[]);
void scanFace(char line[], int numSides);
void displayFaces();