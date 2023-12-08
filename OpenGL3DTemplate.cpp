#pragma comment(lib, "winmm.lib")
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include <glut.h>
#include <irrKlang.h>

using namespace std;
using namespace irrklang;


int   counter = 10;
int   goaldis = -30;
int   treetall = 0;
int   gameTime = 30;
float rotAng = 0;
float rotAngGoal = 0;
float openAng = 55;
float baloonAng = 55;
float thickness = 0.02;
float playerright = 0;
float playerfront = 0;
float x = 0.8;
float y = 0.7;
float z = 0.8;
float cameraz = 0;
float camerax = 0;
float cameraroty = 0;
float camerarotx = -5;//75
bool top = true;
bool side = true;
bool front = true;
bool walk = true;
bool rot = false;
bool rotg = false;
bool grow = false;
bool opena = false;
bool balloon = false;
bool goalTaken = false;
bool goalTik = false;
bool rotright = false;
bool rotleft = false;
bool winsong = false;
bool losesong = true;
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
ISoundEngine* engine = createIrrKlangDevice();
irrklang::ISound* sound = engine->play2D("media/starter.wav", false);

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.0f, float eyeY = 1.0f, float eyeZ = 3.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 300 / 300, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawPlayer() {

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	glPushMatrix();
	glScaled(0.2, 0.2, 0.2);
	glTranslatef(0 + playerright, 4, 15 - playerfront);
	glRotatef(70, 1, 0, 0);
	if (rotleft == true) {
		glRotatef(-45, 0, 0, 1);

	}
	else if (rotright == true) {
		glRotatef(45, 0, 0, 1);

	}

	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidSphere(0.5, 25, 25);//head
	gluCylinder(qobj, 0.2, 0.2, 1, 14, 14);//neck
	glPushMatrix();//body
	glTranslatef(0, 0, 1.3);
	glutSolidCube(1);
	glPushMatrix();//Right arm
	glTranslatef(0.5, 0, 0);
	glRotatef(90, 0, 1, 0);
	gluCylinder(qobj, 0.15, 0.15, 1, 14, 14);
	glPopMatrix();
	glPushMatrix();//Left arm
	glTranslatef(-0.5, 0, 0);
	glRotatef(-90, 0, 1, 0);
	gluCylinder(qobj, 0.15, 0.15, 1, 14, 14);
	glPopMatrix();
	glPushMatrix();//Right leg
	glTranslatef(0.25, 0, 0.25);
	gluCylinder(qobj, 0.15, 0.15, 1, 14, 14);
	glPopMatrix();
	glPushMatrix();//Left leg
	glTranslatef(-0.25, 0, 0.25);
	gluCylinder(qobj, 0.15, 0.15, 1, 14, 14);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void drawGoal() {
	if (goaldis == -30) {

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		glPushMatrix();
		glRotated(rotAngGoal, 0, 1, 0);
		glScaled(0.2, 0.2, 0.2);
		glTranslated(0, 1, goaldis);
		glPushMatrix();// face
		glColor3f(1.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glutSolidSphere(2.0, 50, 50);
		glPopMatrix();

		glColor3f(0.0, 0.0, 0.0);
		glPushMatrix();	    //Left eye
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(-0.7, 2.0, 2.4);
		glutSolidSphere(0.2, 100, 100);

		glPopMatrix();

		glPushMatrix();	    //Right eye
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(0.7, 2.0, 2.4);
		glutSolidSphere(0.2, 100, 25);
		glPopMatrix();

		glColor3f(0.0, 0.0, 0.0);
		glPushMatrix();	// Draw mouth
		glScalef(0.3, 0.3, 0.3);
		glTranslatef(0.0, 1.0, 4.0);
		if (gameTime == 0 && !goalTaken) {

			glRotatef(90, 0.0, 0.0, 1.0);
			goaldis = 0;
		}
		else {
			glRotatef(-90, 0.0, 0.0, 1.0);

		}
		gluPartialDisk(qobj, 0.0, 2.0, 100, 100, 0.0, 180.0);
		glPopMatrix();
		glPopMatrix();
	}
}

void drawGrass(float xShift, float yShift) {
	int numOfBlocksHoriz = 5;
	int numOfBlocksVerti = 37;
	glColor3d(0.3, 0.7, 0.3);
	for (int j = 0; j < numOfBlocksVerti; j++) {
		for (int i = 0; i > -numOfBlocksHoriz; i--) {
			glPushMatrix();
			glTranslated(0.5 * i + xShift, 0.5 * thickness + yShift, 0.5 * j);
			glScaled(1.0, thickness, 1.0);
			glutSolidCube(1);
			glPopMatrix();
		}
		for (int i = 0; i < numOfBlocksHoriz; i++) {

			glPushMatrix();
			glTranslated(0.5 * i + xShift, 0.5 * thickness + yShift, 0.5 * j);
			glScaled(1.0, thickness, 1.0);
			glutSolidCube(1);
			glPopMatrix();
		}
	}
	for (int j = 0; j > -numOfBlocksVerti; j--) {
		for (int i = 0; i > -numOfBlocksHoriz; i--) {
			glPushMatrix();
			glTranslated(0.5 * i + xShift, 0.5 * thickness + yShift, 0.5 * j);
			glScaled(1.0, thickness, 1.0);
			glutSolidCube(1);
			glPopMatrix();
		}
		for (int i = 0; i < numOfBlocksHoriz; i++) {

			glPushMatrix();
			glTranslated(0.5 * i + xShift, 0.5 * thickness + yShift, 0.5 * j);
			glScaled(1.0, thickness, 1.0);
			glutSolidCube(1);
			glPopMatrix();
		}
	}
}

void drawWall(float xShift, float yShift, char direction) {
	int numOfBlocks;
	glColor3d(x, y, z);
	glPushMatrix();
	if (direction == 'l') {
		numOfBlocks = -30;
		glTranslated(-2 + xShift, 0.3 * thickness + yShift, -0.8);
		glRotated(-90, 0, 1, 0);
	}
	else {

		if (direction == 'r') {
			numOfBlocks = -30;

			glTranslated(2 + xShift, 0.3 * thickness + yShift, 0.8);
			glRotated(90, 0, 1, 0);
		}
		else {
			numOfBlocks = -5;
			glTranslated(0, 0, -19.3);

		}
	}


	for (int i = 0; i > numOfBlocks; i--) {
		glPushMatrix();//The virtical second part of fence
		glTranslated(0.6 * i + xShift, 0.3 * thickness + yShift, 0.3);
		glRotated(-90, 1, 0, 0);
		glScaled(1.0, thickness, 1.0);
		glutSolidCube(0.3);
		glPushMatrix();
		glTranslated(xShift, 0.3 * thickness + yShift, 0.3);
		glutSolidCube(0.3);
		glPushMatrix();
		glTranslated(xShift, 0.3 * thickness + yShift, 0.3);
		glutSolidCube(0.3);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glPushMatrix();//The virtical first part of fence
		glTranslated(-0.7 * i + xShift, 0.3 * thickness + yShift, 0.3);
		glRotated(-90, 1, 0, 0);
		glScaled(1.0, thickness, 1.0);
		glutSolidCube(0.3);
		glPushMatrix();
		glTranslated(xShift, 0.3 * thickness + yShift, 0.3);
		glutSolidCube(0.3);
		glPushMatrix();
		glTranslated(xShift, 0.3 * thickness + yShift, 0.3);
		glutSolidCube(0.3);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}

	glPopMatrix();

}

void drawFerriswheel() {
	glColor3f(0.9, 0.9f, 0.1f);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);

	glPushMatrix();//circular head of the Ferris wheel
	glTranslated(0, 1.2, -9);

	gluCylinder(qobj, 0.8, 0.8, 0.2, 50, 50);
	glRotated(19.9627, 1, 0, 0);
	for (int i = 0; i < 180; i += 45) {//the inner holders
		glPushMatrix();
		glRotated(rotAng, 0, 1, 1);
		glRotated(45, 1, 0, 0);
		glRotated(45 + i, 0, 1, 0);
		gluCylinder(qobj, 0.78, 0.78, 0.04, 50, 50);

		glPushMatrix();//carts in the Ferris wheel
		glColor3f(0.7, 0.5f, 0.0f);
		glTranslated(0.55, 0.55, 0);
		glutSolidCube(0.08);
		glPopMatrix();
		glColor3f(0.9, 0.9f, 0.1f);

		glRotated((180 + i), 0, 1, 0);
		glPushMatrix();//carts in the Ferris wheel
		glColor3f(0.7, 0.5f, 0.0f);
		glTranslated(0.55, 0.5, 0);
		glutSolidCube(0.08);
		glPopMatrix();
		glColor3f(0.9, 0.9f, 0.1f);
		glPopMatrix();
	}
	glPushMatrix();//right outer holder
	glColor3f(0.2, 0.0f, 0.0f);
	glRotated(45, 1, 1, 0);
	gluCylinder(qobj, 0.08, 0.08, 1.8, 50, 50);
	glPopMatrix();
	glPushMatrix();//left outer holder
	glColor3f(0.2, 0.0f, 0.0f);
	glRotated(-45, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	gluCylinder(qobj, 0.08, 0.08, 1.8, 50, 50);
	glPopMatrix();
	glPopMatrix();


}

void drawCarnivalRide() {
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glPushMatrix();
	glTranslated(0, 0.88, -2);
	glRotated(13, 1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.78, 0);
	glRotated(-120, 1, 0, 0);
	glutSolidCone(0.8, 1, 20, 10);
	glPopMatrix();
	glPushMatrix();//circular head of the Carnival Ride
	glPushMatrix();
	glColor3f(0.3, 0.1f, 0.0f);
	glTranslated(0, 0.78, 0);
	glRotated(75, 1, 0, 0);
	gluCylinder(qobj, 0.8, 0.8, 0.2, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.9, 0.9f, 0.1f);
	glRotated(110, 1, 0, 0);
	for (int i = 0; i < 180; i += 45) {//the inner holders
		glPushMatrix();
		glRotated(rotAng, 0, 1, 1);
		glRotated(45, 1, 0, 0);
		glRotated(45 + i, 0, 1, 0);
		gluCylinder(qobj, 0.78, 0.78, 0.04, 50, 50);

		glPushMatrix();//carts in the Carnival Ride
		glColor3f(0.7, 0.5f, 0.0f);
		glTranslated(0.55, 0.55, 0);
		glutSolidCube(0.08);
		glPopMatrix();
		glColor3f(0.9, 0.9f, 0.1f);

		glRotated((180 + i), 0, 1, 0);
		glPushMatrix();//carts in the Carnival Ride
		glColor3f(0.7, 0.5f, 0.0f);
		glTranslated(0.55, 0.5, 0);
		glutSolidCube(0.08);
		glPopMatrix();
		glColor3f(0.9, 0.9f, 0.1f);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.3, 0.1f, 0.0f);
	glTranslated(0, -0.78, 0);
	glRotated(60, 1, 0, 0);
	gluCylinder(qobj, 0.8, 0.8, 0.2, 50, 50);
	glPopMatrix();
	/*
	glPushMatrix();//right outer holder
	glColor3f(0.2, 0.0f, 0.0f);
	glRotated(45, 1, 1, 0);
	gluCylinder(qobj, 0.08, 0.08, 1.8, 50, 50);
	glPopMatrix();
	glPushMatrix();//left outer holder
	glColor3f(0.2, 0.0f, 0.0f);
	glRotated(-45, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	gluCylinder(qobj, 0.08, 0.08, 1.8, 50, 50);
	*/

	glPopMatrix();
	glPopMatrix();


}

void drawTree(float x_axis, float y_axis) {
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glPushMatrix();
	glTranslated(x_axis, 0.5, y_axis);
	glRotatef(-90, 1, 0, 0);
	glScaled(0.3, 0.3, 0.3);

	glColor3f(0.4, 0.2, 0.0);
	glPushMatrix();	// Draw the trunk
	glRotatef(-90, 1, 0, 0);
	gluCylinder(qobj, 0.02, 0.2, 1, 14, 14);
	glPopMatrix();

	glColor3f(0.0, 0.8, 0.0);
	glPushMatrix();// Draw the leaves
	glTranslatef(0, 0.5, 0);
	glutSolidCone(0.5, 1, 20, 10);
	glTranslatef(0, 0, 0.75);
	glutSolidCone(0.4, 1, 20, 10);
	for (float i = 0; i < treetall; i++) {
		glTranslatef(0, 0, 0.75 - (i * 10));
		glutSolidCone(0.4 - (i / 10), 1, 20, 10);
	}
	glPopMatrix();
	glPopMatrix();
}

void drawTrashCan() {

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glPushMatrix();
	glTranslated(-1.8, 0.5, -1);
	glScaled(0.5, 0.5, 0.5);
	glPushMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glRotated(openAng, 1, 0, 0);
	gluCylinder(qobj, 0.2, 0.2, 0.1, 200, 200);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.75, 0.75, 0.75);
	glRotated(75, 1, 0, 0);
	gluCylinder(qobj, 0.2, 0.2, 1, 14, 14);
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(2.24, 7.7, 0.5);
	glRotated(75, 1, 0, 0);
	glutSolidCube(0.05);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void drawballon(float x, float y) {

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glScaled(0.8, 0.8, 0.8);
	glTranslated(x, y, (baloonAng / 100) - 1.2);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();
	glPushMatrix();
	gluCylinder(qobj, 0.01, 0.01, 1, 50, 50);
	glPushMatrix();
	glTranslated(0, 0.45, 1);
	glColor3f(0, 0, 0);
	glTranslated(0, -0.45, 0);
	gluCylinder(qobj, 0.012, 0.012, 0.4, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void timer() {
	glPushMatrix();
	char gameTimeStr[20];
	sprintf(gameTimeStr, "Time: %d sec", gameTime);
	glPopMatrix();
}

void updateGameTime(int value) {
	if (gameTime > 0) {

		gameTime--;
		if (counter > 0) {

			counter--;
		}
		else {
			if (x == 1) {
				x = 0;
				y = 1;
				z = 0;
			}
			else if (y == 1) {
				x = 0;
				y = 0;
				z = 1;
			}
			else {
				x = 1;
				y = 0;
				z = 0;
			}

			counter = 10;
		}
	}



	glutTimerFunc(1000, updateGameTime, 0);
	glutPostRedisplay();
}

void Anim() {
	if (rot) {

		rotAng += 0.08;
	}
	glutPostRedisplay();

}

void motion(int x) {
	if (grow) {

		if (treetall == 3) {
			treetall = 0;
		}
		else {
			treetall += 1;
		}
	}
	if (opena) {


		if (openAng == 25) {
			openAng = 55;
		}
		else {
			openAng = 25;
		}
	}
	if (rotg) {

		if (rotAngGoal > 360) {
			rotAngGoal = 0;

		}
		else {
			rotAngGoal += 0.1;
		}
	}
	if (balloon) {
		if (baloonAng == 25) {
			baloonAng = 55;
		}
		else {
			baloonAng = 25;
		}
	}
	glutTimerFunc(1000, motion, 0);
	glutPostRedisplay();

}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.205;

	switch (key) {
	case 'p'://front view
		if (front) {
			camera.moveZ(-cameraz);
			cameraz = 0;
			camera.rotateY(-cameraroty);
			camera.rotateX(-camerarotx);
			front = false;
			side = true;
			top = true;
		}
		break;
	case 'l'://top view
		if (top) {
			cameraz += 7;
			camerax += 14;
			camera.moveZ(7);
			camera.moveY(14);

			camera.rotateY(-cameraroty);
			camera.rotateX(-camerarotx - 75);
			camerarotx += 75;
			cameraroty = 0;

			top = false;
			front = true;
			side = true;
		}
		break;
	case 'k'://side view
		if (side) {

			side = false;
			top = true;
			front = true;
		}
		break;
	case 'x':
		balloon = false;
		baloonAng = 55;
		break;

	case 'z':
		grow = false;
		treetall = 0;

		break;

	case 'w':
		if (walk) {
			if (playerfront < 90) {
				if (playerfront < 42 && playerfront> 34) {
					if (playerright < 2 && playerright>-2) {
						goaldis = 0;
						goalTaken = true;
						goalTik = true;
						winsong = true;
						losesong = false;

					}
					playerfront++;
					camera.moveZ(d);
					cameraz += d;

				}
				else if ((playerfront > 13 && playerfront < 29) || (playerfront > 46 && playerfront < 64)) {
					if (playerright < 7 && playerright >4 || playerright > -7 && playerright < -4) {

						playerfront++;
						camera.moveZ(d);
						cameraz += d;

					}
				}
				else {

					playerfront++;
					camera.moveZ(d);
					cameraz += d;

				}
			}
			rotleft = false;
			rotright = false;
		}
		break;
	case 'd':
		if (walk) {
			if (playerright < 6) {
				if (playerfront < 42 && playerfront> 34) {
					if (playerright < 2 && playerright>-2) {

						goaldis = 0;
						goalTaken = true;
					}
					playerright++;
					camera.moveX(-d);
					camerax -= d;


				}
				else if (!(playerfront > 14 && playerfront < 29) && !(playerfront > 47 && playerfront < 64)) {

					playerright++;
					camera.moveX(-d);
					camerax -= d;

				}

			}
			rotright = true;
			rotleft = false;
		}
		break;
	case 's':
		if (walk) {
			if (playerfront > 0) {
				if (playerfront < 42 && playerfront> 34) {
					if (playerright < 2 && playerright>-2) {

						goaldis = 0;
						goalTaken = true;
					}
					playerfront--;
					camera.moveZ(-d);
					cameraz -= d;


				}
				else if ((playerfront > 13 && playerfront < 30) || (playerfront > 47 && playerfront < 65)) {
					if (playerright < 7 && playerright >4 || playerright > -7 && playerright < -4) {

						playerfront--;
						camera.moveZ(-d);
						cameraz -= d;

					}
				}
				else {

					playerfront--;
					camera.moveZ(-d);
					cameraz -= d;

				}

			}
			rotleft = false;
			rotright = false;
		}
		break;
	case 'a':
		if (walk) {
			if (playerright > -6) {
				if (playerfront < 42 && playerfront> 34) {
					if (playerright < 2 && playerright>-2) {

						goaldis = 0;
						goalTaken = true;
					}
					playerright--;
					camera.moveX(d);
					camerax += d;


				}
				else if (!(playerfront > 14 && playerfront < 29) && !(playerfront > 47 && playerfront < 64)) {

					playerright--;
					camera.moveX(d);
					camerax += d;

				}
			}
			rotleft = true;
			rotright = false;
		}

		break;

	case 'c':
		opena = false;
		openAng = 55;

		break;

	case 'v':
		rotAngGoal = 0;
		rotg = false;

		break;

	case 'b':
		rot = false;
		rotAng = 0;

		break;

	case 'n':
		balloon = true;

		break;

	case 'q':
		grow = true;

		break;

	case 'e':

		opena = true;
		break;

	case 'r':
		rotg = true;

		break;

	case 't':
		rot = true;

		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		camerarotx += 1;
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		camerarotx -= 1;

		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		cameraroty += 1;

		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		camerarotx -= 1;

		break;
	}

	glutPostRedisplay();
}

void Display() {
	setupCamera();
	setupLights();

	if (goalTik == true) {
		engine->play2D("media/bell.wav", false);
		goalTik = false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameTime == 0) {
		walk = false;
		goaldis = -30;
		drawGoal();
		if (goalTaken)
		{
			if (winsong) {
				engine->stopAllSounds();

				engine->play2D("media/winning.mp3", false);
				winsong = false;
			}
			glClearColor(0, 1, 0, 0);
		}
		else {
			if (!goalTaken) {
				if (losesong) {
					engine->stopAllSounds();

					engine->play2D("media/losing.mp3", false);
					losesong = false;
				}
				glClearColor(1, 0, 0, 0);
				drawGoal();
			}
		}
	}
	else {
		drawWall(0, 0, 'r');
		drawWall(0, 0, 'l');
		drawWall(0, 0, ' ');
		drawGrass(0.0, 0.0);
		for (int i = 1; i > -11; i -= 2) {

			drawTree(-1.7, i);
			drawTree(1.7, i);
		}
		for (float i = 0.8; i > -16; i -= 4) {

			drawballon(-2.4, i);
			drawballon(2.4, i);
		}
		drawFerriswheel();
		drawCarnivalRide();
		drawTrashCan();
		drawGoal();
		drawPlayer();

	}

	glFlush();
}

void main(int argc, char** argv) {
	sound;

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Amusement Park");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(0, motion, 0);
	glutTimerFunc(0, updateGameTime, 0);
	glutSpecialFunc(Special);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glutMainLoop();
	engine->drop();
}
