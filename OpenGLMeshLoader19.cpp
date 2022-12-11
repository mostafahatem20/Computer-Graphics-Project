#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <assert.h>
#include <random>
#include <Windows.h>
#include <MMSystem.h>

#define DEG2RAD(a) (a * 0.0174532925) //convert
int WIDTH = 1280;
int sound = 0;
int HEIGHT = 700;
int level = 1;
bool levelchanged = true;
int score = 0;
float textx = 0;
float texty = 1;
float textz = 0;
float Xhere = 0;
float Yhere = 0;
float Zhere = 0;
float tryx = 0.45;
float tryy = 0.64;
float tryz = 0.3;

int health = 100;
bool Win = false;
bool gameOver = false;
bool speedPotion = false;
bool tookHealth = false;
bool firstPerson = false;
bool changedPerson = false;
int timepassed = 0;
GLuint tex;
GLuint tex1;
char title[] = "3D GAME";
int translateFire = 2.5;
float translateDoor = -1.5;
int translateTree = 0;
int tryRot = -30;
float xDraw = 0;
float yDraw = 0;
float translatePumkin = 0;
// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 500;
int coins[80][2];
bool coinsTaken[80];
int rocks[80][2];
int trees[40][2];
int fire[30][2];

bool fireTranslate[30];
bool stoneTranslate[80];
bool treeTranslate[80];
bool coinsTranslate[80];

float xGirl = 0;
float yGirl = 0;
float jumpGirl = 0;
int jump = 0;
int rotGirl = 0;
int stoneRot = 0;
float translateCoins = 0.5;
bool animatepumkin = false;
bool animatepumkin2 = false;
bool animatepumkin3 = false;
bool fireResistance = false;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
	Vector operator+(Vector v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator-(Vector v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector operator*(float n) {
		return Vector(x * n, y * n, z * n);
	}

	Vector operator/(float n) {
		return Vector(x / n, y / n, z / n);
	}

	Vector unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector cross(Vector v) {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

Vector Eye(0.12, 1.9, 2);
Vector At(0.14, 1.9, 0.08);
Vector Up(0.01, 1, -0.01);

int cameraZoom = 0;

// Model Variables
Model_3DS model_tree;
Model_3DS model_stone;
Model_3DS model_fire;
Model_3DS model_girl;
Model_3DS model_chili;
Model_3DS model_coin;
Model_3DS model_door;
Model_3DS model_pumkin;

Model_3DS model_emerald;
// Textures
GLTexture tex_ground;
GLTexture tex_ground2;
void moveX(float d) {
	Vector right = Up.cross(At - Eye).unit();
	Vector temp = right * d;
	Eye = Eye + temp;
	At = At + temp;
}

void moveY(float d) {
	Vector temp = Up.unit() * d;
	Eye = Eye + temp;
	At = At + temp;
}

void moveZ(float d) {
	Vector view = (At - Eye).unit();
	Vector temp = view * d;
	Eye = Eye + temp;
	At = At + temp;
}

void rotateX(float a) {
	Vector view = (At - Eye).unit();
	Vector right = Up.cross(view).unit();
	Vector temp = Up * sin(DEG2RAD(a));
	view = view * cos(DEG2RAD(a)) + temp;
	Up = view.cross(right);
	At = Eye + view;
}

void rotateY(float a) {
	Vector view = (At - Eye).unit();
	Vector right = Up.cross(view).unit();
	Vector temp = right * sin(DEG2RAD(a));
	view = view * cos(DEG2RAD(a)) + temp;
	right = view.cross(Up);
	At = Eye + view;
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.0f, 0.0f, 0.0, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	//GLfloat ambient1[] = { 0.7f, 0.0f, 0.0f, 1.0f };
//	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//GLfloat diffuse1[] = { 1.0, 0.0, 0.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);


	// Define Light source 0 Specular light
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//GLfloat light_position1[] = { 0.0f, 0.0f, 0.2f, 0.0f };
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	//GLfloat spotCUtOff[] = { 10.0f };
	//glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotCUtOff);

	//GLfloat SPOTDIRECTION[] = { 0.0f, -1.0f, 0.0f };
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SPOTDIRECTION);

//	GLfloat SPOTEXPONENT[] = { 2.0f };
	//glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, SPOTEXPONENT);


	glEnable(GL_LIGHT0);

	//float gotHere = timepassed % 63 +coins[0][0];
	//gotHere = gotHere / 1000;
	//printf("%f \n", gotHere);


	GLfloat light0_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light0_ambient1[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	GLfloat light0_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLfloat light0_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLfloat light0_position[] = { -2.0f, 4.0f, 0.0f, 1.0f };


	GLfloat light0_constant_attenuation = 0.0f;
	GLfloat light0_linear_attenuation = 0.1f;
	GLfloat light0_quadratic_attenuation = 0.0f;
	//glLightfv(GL_LIGHT2, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light0_specular);
	//	glLightfv(GL_LIGHT2, GL_POSITION, light0_position);
	//	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, light0_constant_attenuation);
		//glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, light0_linear_attenuation);
		//glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, light0_quadratic_attenuation);
		//glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spotCUtOff);
		//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, SPOTDIRECTION);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.3f);
	glLightfv(GL_LIGHT2, GL_POSITION, light0_position);

	GLfloat spotexp = 5.0f;
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128.0);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 40.0);
	GLfloat spot_direction[] = { 0, -1.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	glLightfv(GL_LIGHT3, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light0_position);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 128.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);

	glLightfv(GL_LIGHT4, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light0_position);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 128.0);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);


	glLightfv(GL_LIGHT5, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT5, GL_POSITION, light0_position);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 128.0);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);


	glLightfv(GL_LIGHT6, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT6, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT6, GL_POSITION, light0_position);
	glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.3f);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 128.0);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_direction);


}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy: Angle between the bottom and top of the projectors, in degrees. //
	// aspectRatio: Ratio of width to height of the clipping plane. //
	// zNear and zFar: Specify the front and back clipping planes distances from camera. //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera. //
	// AT (ax, ay, az): denotes the direction where the camera is aiming at. //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera. //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

bool canEnter(int x1, int y1, int z1) {

	if ((x1 * x1) + (z1 * z1) >= 10000 - 20) {
		return false;
	}
	//for stones and tree and end of earth
	if (level == 1) {
		for (int i = 0; i < 80; i++) {
			bool Xcommon = false;
			bool Zcommon = false;
			if (x1 + 0.25 >= rocks[i][0] - 0.25 && x1 - 0.25 <= rocks[i][0] - 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 >= rocks[i][0] - 0.25 && x1 + 0.25 <= rocks[i][0] + 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] + 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] - 0.25) {
				Xcommon = true;
			}

			if (z1 + 0.25 >= rocks[i][1] - 0.25 && z1 - 0.25 <= rocks[i][1] - 0.25) {
				Zcommon = true;

			}
			else if (z1 - 0.25 >= rocks[i][1] - 0.25 && z1 + 0.25 <= rocks[i][1] + 0.25) {
				Zcommon = true;
			}
			else if (z1 - 0.25 <= rocks[i][1] - 0.25 && z1 + 0.25 >= rocks[i][1] + 0.25) {
				Zcommon = true;
			}
			else if (z1 - 0.25 <= rocks[i][1] - 0.25 && z1 + 0.25 >= rocks[i][1] - 0.25) {
				Zcommon = true;
			}

			if (xGirl + 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] - 0.25) {
				Xcommon = true;
			}
			if (yGirl + 0.25 <= rocks[i][1] - 0.25 && z1 - 0.25 >= rocks[i][1] + 0.25) {
				Zcommon = true;
			}
			if (Xcommon && Zcommon) {
				health -= 5;
				sound = 4;
				stoneTranslate[i] = true;
				if (health <= 0) {
					health = 0;
					gameOver = true;

				}

				return false;
			}
			if ((!Xcommon && !Zcommon) && stoneTranslate[i]) {
				stoneTranslate[i] = false;
			}
		}
	}
	if (level == 1) {
		for (int i = 0; i < 80; i++) {
			bool Xcommon = false;
			bool Zcommon = false;
			if (x1 + 0.25 >= trees[i][0] - 0.25 && x1 - 0.25 <= trees[i][0] - 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 >= trees[i][0] - 0.25 && x1 + 0.25 <= trees[i][0] + 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] + 0.25) {
				Xcommon = true;
			}
			else if (x1 - 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] - 0.25) {
				Xcommon = true;
			}

			if (z1 + 0.25 >= trees[i][1] - 0.25 && z1 - 0.25 <= trees[i][1] - 0.25) {
				Zcommon = true;

			}
			else if (z1 - 0.25 >= trees[i][1] - 0.25 && z1 + 0.25 <= trees[i][1] + 0.25) {
				Zcommon = true;
			}
			else if (z1 - 0.25 <= trees[i][1] - 0.25 && z1 + 0.25 >= trees[i][1] + 0.25) {
				Zcommon = true;
			}
			else if (z1 - 0.25 <= trees[i][1] - 0.25 && z1 + 0.25 >= trees[i][1] - 0.25) {
				Zcommon = true;
			}

			if (xGirl + 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] - 0.25) {
				Xcommon = true;
			}
			if (yGirl + 0.25 <= trees[i][1] - 0.25 && z1 - 0.25 >= trees[i][1] + 0.25) {
				Zcommon = true;
			}
			if (Xcommon && Zcommon) {
				treeTranslate[i] = true;
				sound = 3;
				return false;
			}
			if ((!Xcommon && !Zcommon) && treeTranslate[i]) {
				treeTranslate[i] = false;
			}
		}
	}

	return true;

}


bool canJump(int x1, int y1, int z1) {

	if ((x1 * x1) + (z1 * z1) >= 10000 - 20) {
		return false;
	}
	//for stones and tree and end of earth
	for (int i = 0; i < 80; i++) {
		bool Xcommon = false;
		bool Zcommon = false;

		if (x1 + 0.25 >= rocks[i][0] - 0.25 && x1 - 0.25 <= rocks[i][0] - 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 >= rocks[i][0] - 0.25 && x1 + 0.25 <= rocks[i][0] + 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] + 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] - 0.25) {
			Xcommon = true;
		}

		if (z1 + 0.25 >= rocks[i][1] - 0.25 && z1 - 0.25 <= rocks[i][1] - 0.25) {
			Zcommon = true;

		}
		else if (z1 - 0.25 >= rocks[i][1] - 0.25 && z1 + 0.25 <= rocks[i][1] + 0.25) {
			Zcommon = true;
		}
		else if (z1 - 0.25 <= rocks[i][1] - 0.25 && z1 + 0.25 >= rocks[i][1] + 0.25) {
			Zcommon = true;
		}
		else if (z1 - 0.25 <= rocks[i][1] - 0.25 && z1 + 0.25 >= rocks[i][1] - 0.25) {
			Zcommon = true;
		}

		if (xGirl + 0.25 <= rocks[i][0] - 0.25 && x1 + 0.25 >= rocks[i][0] - 0.25) {
			Xcommon = true;
		}
		if (yGirl + 0.25 <= rocks[i][1] - 0.25 && z1 - 0.25 >= rocks[i][1] + 0.25) {
			Zcommon = true;
		}
		if (Xcommon && Zcommon) {
			health -= 5;
			sound = 4;
			stoneTranslate[i] = true;
			if (health <= 0) {
				health = 0;
				gameOver = true;
			}

			return false;
		}
		if ((!Xcommon && !Zcommon) && stoneTranslate[i]) {
			stoneTranslate[i] = false;
		}
	}

	for (int i = 0; i < 80; i++) {
		bool Xcommon = false;
		bool Zcommon = false;
		bool treeHere = false;
		if (x1 + 0.25 >= trees[i][0] - 0.25 && x1 - 0.25 <= trees[i][0] - 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 >= trees[i][0] - 0.25 && x1 + 0.25 <= trees[i][0] + 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] + 0.25) {
			Xcommon = true;
		}
		else if (x1 - 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] - 0.25) {
			Xcommon = true;
		}

		if (z1 + 0.25 >= trees[i][1] - 0.25 && z1 - 0.25 <= trees[i][1] - 0.25) {
			Zcommon = true;

		}
		else if (z1 - 0.25 >= trees[i][1] - 0.25 && z1 + 0.25 <= trees[i][1] + 0.25) {
			Zcommon = true;
		}
		else if (z1 - 0.25 <= trees[i][1] - 0.25 && z1 + 0.25 >= trees[i][1] + 0.25) {
			Zcommon = true;
		}
		else if (z1 - 0.25 <= trees[i][1] - 0.25 && z1 + 0.25 >= trees[i][1] - 0.25) {
			Zcommon = true;
		}

		if (xGirl + 0.25 <= trees[i][0] - 0.25 && x1 + 0.25 >= trees[i][0] - 0.25) {
			Xcommon = true;
		}
		if (yGirl + 0.25 <= trees[i][1] - 0.25 && z1 - 0.25 >= trees[i][1] + 0.25) {
			Zcommon = true;
		}
		if (yGirl >= trees[i][1] && trees[i][1] >= z1) {
			treeHere = true;
		}
		if (Xcommon && Zcommon) {
			treeTranslate[i] = true;
			sound = 3;
			return false;
		}
		if (treeHere) {
			return false;
		}

		if ((!Xcommon && !Zcommon) && treeTranslate[i]) {
			treeTranslate[i] = false;
		}

	}

	return true;

}


bool camerCanEnterInc() {
	if ((Eye.x * Eye.x) + (Eye.y * Eye.y) + (Eye.z * Eye.z) >= 9900) {
		if (Eye.x > 0) {
			Eye.x = Eye.x - 1;
		}
		else {
			Eye.x = Eye.x + 1;
		}

		if (Eye.z > 0) {
			Eye.z = Eye.z - 1;
		}
		else {
			Eye.z = Eye.z + 1;
		}

		if (At.x > 0) {
			At.x = At.x - 1;
		}
		else {
			At.x = At.x + 1;
		}

		if (At.z > 0) {
			At.z = At.z - 1;
		}
		else {
			At.z = At.z + 1;
		}

		return false;
	}
	return true;
}

bool camerCanEnterDec() {
	if (Eye.y <= 1) {
		return false;
	}
	return true;
}
void checkPumkin() {
	bool Xcommon = false;
	bool Zcommon = false;
	bool Xcommon2 = false;
	bool Zcommon2 = false;

	if (xGirl + 0.5 >= -34 && xGirl - 0.5 <= -36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 >= -34 && xGirl + 0.5 <= -36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= -34 && xGirl + 0.5 >= -36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= -34 && xGirl + 0.5 >= -36) {
		Xcommon = true;
	}
	if (yGirl + 0.5 >= -34 && yGirl - 0.5 <= -36) {
		Zcommon = true;

	}
	else if (yGirl - 0.5 >= -34 && yGirl + 0.5 <= -36) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= -34 && yGirl + 0.5 >= -36) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= -34 && yGirl + 0.5 >= -36) {
		Zcommon = true;
	}
	if (Xcommon && Zcommon && !speedPotion) {
		speedPotion = true;
		sound = 8;

	}

	if (xGirl + 0.5 >= -38 && xGirl - 0.5 <= -32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 >= -38 && xGirl + 0.5 <= -32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= -38 && xGirl + 0.5 >= -32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= -38 && xGirl + 0.5 >= -32) {
		Xcommon2 = true;
	}
	if (yGirl + 0.5 >= -38 && yGirl - 0.5 <= -32) {
		Zcommon2 = true;

	}
	else if (yGirl - 0.5 >= -38 && yGirl + 0.5 <= -32) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= -38 && yGirl + 0.5 >= -32) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= -38 && yGirl + 0.5 >= -32) {
		Zcommon2 = true;
	}
	if (Xcommon2 && Zcommon2 && !speedPotion) {
		animatepumkin = true;
	}

	if (!Xcommon2 && !Zcommon2 && !speedPotion) {
		animatepumkin = false;
	}

}

void checkEmerald() {
	bool Xcommon = false;
	bool Zcommon = false;
	bool Xcommon2 = false;
	bool Zcommon2 = false;

	if (xGirl + 0.5 >= -1 && xGirl - 0.5 <= 1) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 >= -1 && xGirl + 0.5 <= 1) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= -1 && xGirl + 0.5 >= 1) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= -1 && xGirl + 0.5 >= 1) {
		Xcommon = true;
	}
	if (yGirl + 0.5 >= -1 && yGirl - 0.5 <= 1) {
		Zcommon = true;

	}
	else if (yGirl - 0.5 >= -1 && yGirl + 0.5 <= 1) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= -1 && yGirl + 0.5 >= 1) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= -1 && yGirl + 0.5 >= 1) {
		Zcommon = true;
	}
	if (Xcommon && Zcommon ) {
		sound = 1;
		Win = true;
	}
}

void checkPumkin2() {
	bool Xcommon = false;
	bool Zcommon = false;
	bool Xcommon2 = false;
	bool Zcommon2 = false;

	if (xGirl + 0.5 >= 34 && xGirl - 0.5 <= 36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 >= 34 && xGirl + 0.5 <= 36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= 34 && xGirl + 0.5 >= 36) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= 34 && xGirl + 0.5 >= 36) {
		Xcommon = true;
	}
	if (yGirl + 0.5 >= 34 && yGirl - 0.5 <= 36) {
		Zcommon = true;

	}
	else if (yGirl - 0.5 >= 34 && yGirl + 0.5 <= 36) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= 34 && yGirl + 0.5 >= 36) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= 34 && yGirl + 0.5 >= 36) {
		Zcommon = true;
	}
	if (Xcommon && Zcommon && !tookHealth) {
		health += 10;
		tookHealth = true;
		sound = 5;
	}

	if (xGirl + 0.5 >= 38 && xGirl - 0.5 <= 32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 >= 38 && xGirl + 0.5 <= 32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= 38 && xGirl + 0.5 >= 32) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= 38 && xGirl + 0.5 >= 32) {
		Xcommon2 = true;
	}
	if (yGirl + 0.5 >= 38 && yGirl - 0.5 <= 32) {
		Zcommon2 = true;

	}
	else if (yGirl - 0.5 >= 38 && yGirl + 0.5 <= 32) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= 38 && yGirl + 0.5 >= 32) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= 38 && yGirl + 0.5 >= 32) {
		Zcommon2 = true;
	}
	if (Xcommon2 && Zcommon2 && !tookHealth) {
		animatepumkin2 = true;
	}

	if (!Xcommon2 && !Zcommon2 && !tookHealth) {
		animatepumkin2 = false;
	}

}

void checkPumkin3() {
	bool Xcommon = false;
	bool Zcommon = false;
	bool Xcommon2 = false;
	bool Zcommon2 = false;

	if (xGirl + 0.5 >= 19 && xGirl - 0.5 <= 21) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 >= 19 && xGirl + 0.5 <= 21) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= 19 && xGirl + 0.5 >= 21) {
		Xcommon = true;
	}
	else if (xGirl - 0.5 <= 19 && xGirl + 0.5 >= 21) {
		Xcommon = true;
	}
	if (yGirl + 0.5 >= 19 && yGirl - 0.5 <= 21) {
		Zcommon = true;

	}
	else if (yGirl - 0.5 >= 19 && yGirl + 0.5 <= 21) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= 19 && yGirl + 0.5 >= 21) {
		Zcommon = true;
	}
	else if (yGirl - 0.5 <= 19 && yGirl + 0.5 >= 21) {
		Zcommon = true;
	}
	if (Xcommon && Zcommon && !fireResistance) {
		fireResistance = true;
		sound = 7;
	}

	if (xGirl + 0.5 >= 23 && xGirl - 0.5 <= 17) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 >= 23 && xGirl + 0.5 <= 17) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= 23 && xGirl + 0.5 >= 17) {
		Xcommon2 = true;
	}
	else if (xGirl - 0.5 <= 23 && xGirl + 0.5 >= 17) {
		Xcommon2 = true;
	}
	if (yGirl + 0.5 >= 23 && yGirl - 0.5 <= 17) {
		Zcommon2 = true;

	}
	else if (yGirl - 0.5 >= 23 && yGirl + 0.5 <= 17) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= 23 && yGirl + 0.5 >= 17) {
		Zcommon2 = true;
	}
	else if (yGirl - 0.5 <= 23 && yGirl + 0.5 >= 17) {
		Zcommon2 = true;
	}
	if (Xcommon2 && Zcommon2 && !fireResistance) {
		animatepumkin3 = true;
	}

	if (!Xcommon2 && !Zcommon2 && !fireResistance) {
		animatepumkin3 = false;
	}

}


void checkCoin() {

	for (int i = 0; i < 80; i++) {
		bool Xcommon = false;
		bool Zcommon = false;
		bool Xcommon2 = false;
		bool Zcommon2 = false;

		if (xGirl + 0.5 >= coins[i][0] - 0.4 && xGirl - 0.5 <= coins[i][0] - 0.4) {
			Xcommon = true;

		}
		else if (xGirl - 0.5 >= coins[i][0] - 0.4 && xGirl + 0.5 <= coins[i][0] + 0.4) {
			Xcommon = true;
		}
		else if (xGirl - 0.5 <= coins[i][0] - 0.4 && xGirl + 0.5 >= coins[i][0] + 0.4) {
			Xcommon = true;
		}
		else if (xGirl - 0.5 <= coins[i][0] - 0.4 && xGirl + 0.5 >= coins[i][0] - 0.4) {
			Xcommon = true;
		}

		if (yGirl + 0.5 >= coins[i][1] - 0.16 && yGirl - 0.5 <= coins[i][1] - 0.16) {
			Zcommon = true;

		}
		else if (yGirl - 0.5 >= coins[i][1] - 0.16 && yGirl + 0.5 <= coins[i][1] + 0.16) {
			Zcommon = true;
		}
		else if (yGirl - 0.5 <= coins[i][1] - 0.16 && yGirl + 0.5 >= coins[i][1] + 0.16) {
			Zcommon = true;
		}
		else if (yGirl - 0.5 <= coins[i][1] - 0.16 && yGirl + 0.5 >= coins[i][1] - 0.16) {
			Zcommon = true;
		}
		if (Xcommon && Zcommon && !coinsTaken[i]) {
			coinsTaken[i] = true;
			score += 10;
			sound = 2;
		}

		if (xGirl + 1.5 >= coins[i][0] - 1.4 && xGirl - 1.5 <= coins[i][0] - 1.4) {
			Xcommon2 = true;

		}
		else if (xGirl - 1.5 >= coins[i][0] - 1.4 && xGirl + 1.5 <= coins[i][0] + 1.4) {
			Xcommon2 = true;
		}
		else if (xGirl - 1.5 <= coins[i][0] - 1.4 && xGirl + 1.5 >= coins[i][0] + 1.4) {
			Xcommon2 = true;
		}
		else if (xGirl - 1.5 <= coins[i][0] - 1.4 && xGirl + 1.5 >= coins[i][0] - 1.4) {
			Xcommon2 = true;
		}

		if (yGirl + 1.5 >= coins[i][1] - 1.16 && yGirl - 1.5 <= coins[i][1] - 1.16) {
			Zcommon2 = true;

		}
		else if (yGirl - 1.5 >= coins[i][1] - 1.16 && yGirl + 1.5 <= coins[i][1] + 1.16) {
			Zcommon2 = true;
		}
		else if (yGirl - 1.5 <= coins[i][1] - 1.16 && yGirl + 1.5 >= coins[i][1] + 1.16) {
			Zcommon2 = true;
		}
		else if (yGirl - 1.5 <= coins[i][1] - 1.16 && yGirl + 1.5 >= coins[i][1] - 1.16) {
			Zcommon2 = true;
		}
		if (Xcommon2 && Zcommon2 && !coinsTaken[i]) {
			coinsTranslate[i] = true;
		}
		if (!Xcommon2 && !Zcommon2 && coinsTranslate[i]) {
			coinsTranslate[i] = false;
		}
	}

}

void checkFire() {

	for (int i = 0; i < 30; i++) {
		bool Xcommon = false;
		bool Zcommon = false;
		if (xGirl + 0.5 >= fire[i][0] - 2.2 && xGirl - 0.5 <= fire[i][0] - 2.2) {
			Xcommon = true;

		}
		else if (xGirl - 0.5 >= fire[i][0] - 2.2 && xGirl + 0.5 <= fire[i][0] + 2.2) {
			Xcommon = true;
		}
		else if (xGirl - 0.5 <= fire[i][0] - 2.2 && xGirl + 0.5 >= fire[i][0] + 2.2) {
			Xcommon = true;
		}
		else if (xGirl - 0.5 <= fire[i][0] - 2.2 && xGirl + 0.5 >= fire[i][0] - 2.2) {
			Xcommon = true;
		}

		if (yGirl + 0.5 >= fire[i][1] - 2.2 && yGirl - 0.5 <= fire[i][1] - 2.2) {
			Zcommon = true;

		}
		else if (yGirl - 0.5 >= fire[i][1] - 2.2 && yGirl + 0.5 <= fire[i][1] + 2.2) {
			Zcommon = true;
		}
		else if (yGirl - 0.5 <= fire[i][1] - 2.2 && yGirl + 0.5 >= fire[i][1] + 2.2) {
			Zcommon = true;
		}
		else if (yGirl - 0.5 <= fire[i][1] - 2.2 && yGirl + 0.5 >= fire[i][1] - 2.2) {
			Zcommon = true;
		}
		if (Xcommon && Zcommon) {
			if (!fireResistance) {
				health -= 10;
				sound = 4;
			}
			fireTranslate[i] = true;
			printf("health: %d", health);
			if (health <= 0) {
				health = 0;
				Eye.x = 17.43;
				Eye.y = 19.32;
				Eye.z = 42;
				At.x = 17.45;
				At.y = 19.32;
				At.z = 40.08;
				Up.x = 0.01;
				Up.y = 1;
				Up.z = -0.01;
				glLoadIdentity(); //Clear Model_View Matrix
				gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
				glutPostRedisplay();

				gameOver = true;
			}
		}
	}


}
//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	levelchanged = false;
	// Loading Model files
	model_tree.Load("Models/tree/Tree1.3ds");
	model_stone.Load("Models/stone/Stone.3DS");
	model_girl.Load("Models/girl/girl.3DS");
	model_chili.Load("Models/chili/Pepper Chili.3DS");
	model_coin.Load("Models/coin/Coin.3DS");
	model_door.Load("Models/door/Door.3ds");
	model_pumkin.Load("Models/pumkin/Pumpkin.3DS");
	model_fire.Load("Models/fire/Plant house N030919.3ds");
	model_emerald.Load("Models/emerald/Lamp LED Holiday PINYA N221119.3ds");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_ground2.Load("Textures/lava-sky-3.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&tex1, "Textures/ground2.bmp", true);

}
void checkDoor() {



	if ((xGirl >= 15 && xGirl <= 23) && (yGirl <= -46 && yGirl >= -53)) {
		if (translateDoor < 2) {
			translateDoor += 0.1;
		}
	}

	if ((xGirl >= 18 && xGirl <= 20) && (yGirl <= -49 && yGirl >= -50)) {
		changedPerson = true;
		firstPerson = false;
		level = 2;
		levelchanged = true;

		for (int i = 0; i < 80; i++) {
			coinsTaken[i] = false;
		}

		xGirl = -20;
		yGirl = 50;

		Eye.x = xGirl + 0.12;
		Eye.y = 1.9;
		Eye.z = yGirl + 2.0;

		At.x = xGirl;
		At.y = 1.9;
		At.z = yGirl + 1.1;

		Up.x = 0.01;
		Up.y = 1;
		Up.z = 0;

		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
		moveZ(-1.1);
		moveZ(-1.1);
		moveZ(-1.1);
		glutPostRedisplay();
	}
	if (level == 2) {
		sound = 6;
	}
}



//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING); // Disable lighting

	glColor3f(0.6, 0.6, 0.6); // Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D); // Enable 2D texturing

	if (level == 1) {
		glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]); // Bind the ground texture
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_ground2.texture[0]); // Bind the ground texture
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0); // Set quad normal direction.
	glTexCoord2f(0, 0); // Set tex coordinates ( Using (0,0) ->=(5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-120, 0, -100);
	glTexCoord2f(5, 0);
	glVertex3f(120, 0, -100);
	glTexCoord2f(5, 5);
	glVertex3f(120, 0, 100);
	glTexCoord2f(0, 5);
	glVertex3f(-120, 0, 100);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING); // Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1); // Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================
void displayScore() {
	glPushAttrib(GL_CURRENT_BIT);
	if (level == 1) {
		glColor3f(0, 0, 0);
	}
	else {
		glColor3f(1, 1, 1);
	}
	char* c;

	if (level == 1) {
		glColor3f(0, 0, 0);
	}
	else {
		glColor3f(1, 1, 1);
	}
	char mystring1[60];
	if (Win) {
		sprintf(mystring1, "The score is: %d   The health is: %d \n You Won", score, health);
	}
	else if (gameOver) {
		sprintf(mystring1, "The score is: %d   The health is: %d \n Game Over", score, health);
	}
	else {
		sprintf(mystring1, "The score is: %d   The health is: %d \n", score, health);
	}
	glPushAttrib(GL_CURRENT_BIT);
	if (level == 1) {
		glColor3f(0, 0, 0);
	}
	else {
		glColor3b(1, 1, 1);
	}
	glRasterPos3f(0, 1.6, 0);

	for (c = mystring1; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

	}
	glPopAttrib();
}

void firstPart() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightIntensity1[] = { 0.5, 0.5, 0.5, 1.0f };
	GLfloat lightIntensity[] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	if (level == 2) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	}
	else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity1);
	}


	if (Win || gameOver) {
		Eye.x = 17.43;
		Eye.y = 19.32;
		Eye.z = 42;
		At.x = 17.45;
		At.y = 19.32;
		At.z = 40.08;
		Up.x = 0.01;
		Up.y = 1;
		Up.z = -0.01;
		glLoadIdentity(); //Clear Model_View Matrix
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
		glutPostRedisplay();
		sound = 1;
	}


}

void textPart() {
	glPushMatrix();
	glTranslatef(0, 25, 0);
	displayScore();
	glPopMatrix();

}

void pumkinPart() {
	if (!speedPotion) {
		glPushMatrix();
		if (animatepumkin) {
			glTranslatef(-35, translatePumkin, -35);
		}
		else {
			glTranslatef(-35, 0, -35);
		}
		glScalef(0.2, 0.2, 0.2);
		glRotated(-90, 1, 0, 0);
		model_pumkin.Draw();
		glPopMatrix();
	}
}

void pumkin2Part() {
	if (!tookHealth) {
		glPushMatrix();
		if (animatepumkin2) {
			glTranslatef(35, translatePumkin, 35);
		}
		else {
			glTranslatef(35, 0, 35);
		}
		//glScalef(0.2, 0.2, 0.2);
		glRotated(-90, 1, 0, 0);
		//model_pumkin.Draw();
		glColor3f(1, 1, 0);
		glutSolidCube(0.5);
		glPopMatrix();
	}
}


void pumkin3Part() {
	if (!fireResistance) {
		glPushMatrix();
		if (animatepumkin3) {
			glTranslatef(20, translatePumkin, 20);
		}
		else {
			glTranslatef(20, 0, 20);
		}
		glScalef(0.2, 0.2, 0.2);
		glRotated(-90, 1, 0, 0);
		model_chili.Draw();
		glPopMatrix();
	}
}


void level1Part() {
	//Draw girl
	glPushMatrix();
	glTranslatef(xGirl, jumpGirl, yGirl);

	//	glTranslatef(-xGirl, -jumpGirl, -yGirl);
	//	glRotated(rotGirl, 0, 1, 0);
	//	glTranslatef(-xGirl, -jumpGirl, -yGirl);

		//glScalef(0.7, 0.7, 0.7);
	model_girl.Draw();
	glPopMatrix();


	for (int i = 0; i < 80; i++) {
		if (!coinsTaken[i]) {
			glPushMatrix();
			if (coinsTranslate[i]) {
				glTranslatef(coins[i][0], translateCoins, coins[i][1]);
			}
			else {
				glTranslatef(coins[i][0], 0.5, coins[i][1]);
			}
			glScalef(0.1, 0.1, 0.1);
			glRotated(90, 1, 0, 0);
			model_coin.Draw();
			glPopMatrix();
		}
	}
	glPushMatrix();
	glTranslatef(20, translateDoor, -50);
	glScalef(1.7, 1.7, 1.7);
	model_door.Draw();
	glPopMatrix();

	//Draw stone
	for (int i = 1; i < 80; i++) {
		glPushMatrix();
		glTranslatef(rocks[i][0] + 0.5, 0, rocks[i][1]);
		if (stoneTranslate[i] == true) {
			//	glTranslatef(-rocks[i][0] - 0.5, 0, -rocks[i][1]);
			glRotated(stoneRot, 0, 1, 0);
			//		glTranslatef(rocks[i][0] + 0.5, 0, rocks[i][1]);
				//	stoneTranslate[i] = false;
		}
		//glScalef(0.7, 0.7, 0.7);
		model_stone.Draw();
		glPopMatrix();
	}

	// Draw Tree Model
	for (int i = 1; i < 40; i++) {

		if (treeTranslate[i] && translateTree == 100) {
		}
		else {
			glPushMatrix();
			glTranslatef(trees[i][0], 0, trees[i][1]);
			glScalef(0.2, 0.2, 0.2);
			model_tree.Draw();
			glPopMatrix();
		}
	}

}

void draw2D() {
	if (level == 1) {
		glColor3f(0, 0, 0);
	}

	else {
		glColor3f(1, 1, 1);
	}

	if (!firstPerson && !changedPerson) {
		glPushMatrix();

		glTranslatef(At.x + 1.1, At.y + 0.63, At.z);
		glScaled(2, 0.5, 1);
		glRotatef(tryRot, -0.8, 1, 0);
		glutSolidCube(0.5);
		glPopMatrix();

		int c = 0;
		for (int i = 0; i < health; i = i + 5) {
			glColor3f(1, 0, 0);
			glPushMatrix();
			glTranslatef(At.x + tryx + 0.03 * c, At.y + tryy, At.z + tryz);
			glRotatef(tryRot, -0.8, 1, 0);
			glutSolidCube(0.02);
			glPopMatrix();
			c += 1;
		}
		glColor3f(1, 1, 0);
		int e = 0;
		int l = 1;
		for (int i = 0; i < score; i = i + 10) {
			glColor3f(1, 1, 0);
			glPushMatrix();

			glTranslatef(At.x + tryx + 0.03 * e, At.y + tryy - 0.035 * l, At.z + tryz);
			glRotatef(tryRot, -0.8, 1, 0);
			glScalef(0.005, 0.005, 0.005);
			glRotated(90, 1, 0, 0);
			model_coin.Draw();
			glPopMatrix();
			e += 1;
			if (e == 20) {
				e = 0;
				l += 1;
			}
		}
	}
	else if (firstPerson) {
		glPushMatrix();
		glTranslatef(xGirl + 0.99, 2.75, yGirl - 1.7);
		glRotated(1, 47, 0, -63);
		glScaled(2, 0.5, 1);
		glutSolidCube(0.5);
		glPopMatrix();

		int c = 0;
		for (int i = 0; i < health; i = i + 5) {
			glColor3f(1, 0, 0);
			glPushMatrix();
			glTranslatef(xGirl + 0.44 + 0.03 * c, 2.6, yGirl - 1 + 0.05);
			//glTranslatef(At.x + tryx + 0.03 * c, At.y + tryy, At.z + tryz);
			//glRotatef(tryRot, -0.8, 1, 0);
			glRotated(1, 47, 0, -63);
			glutSolidCube(0.02);
			glPopMatrix();
			c += 1;
		}

		glColor3f(1, 1, 0);
		int e = 0;
		int l = 1;
		for (int i = 0; i < score; i = i + 10) {
			glColor3f(1, 1, 0);
			glPushMatrix();
			glTranslatef(xGirl + 0.44 + 0.03 * e, 2.6 - 0.035 * l, yGirl - 1);
			glRotated(1, 47, 0, -63);
			glScalef(0.005, 0.005, 0.005);
			glRotated(90, 1, 0, 0);
			model_coin.Draw();
			glPopMatrix();
			e += 1;
			if (e == 20) {
				e = 0;
				l += 1;
			}
		}
	}
	else if (changedPerson) {
		glPushMatrix();
		glTranslatef(At.x + 0.9 + 0.06, At.y + 0.73 + 0.08, At.z - 1.7);
		glRotated(1, 47, 0, -63);
		glScaled(2, 0.5, 1);
		glutSolidCube(0.5);
		glPopMatrix();

		int c = 0;
		for (int i = 0; i < health; i = i + 5) {
			glColor3f(1, 0, 0);

			glPushMatrix();

			glTranslatef(At.x + 0.35 + 0.06 + 0.03 * c, At.y + 0.04 + tryy, At.z - 1);
			glRotated(1, 47, 0, -63);
			//glTranslatef(At.x + tryx + 0.03 * c, At.y + tryy, At.z + tryz);
			//glRotatef(tryRot, -0.8, 1, 0);
			glutSolidCube(0.02);
			glPopMatrix();
			c += 1;
		}

		glColor3f(1, 1, 0);
		int e = 0;
		int l = 1;
		for (int i = 0; i < score; i = i + 10) {
			glColor3f(1, 1, 0);
			glPushMatrix();
			glTranslatef(At.x + 0.35 + 0.06 + 0.03 * e, At.y + 0.04 + tryy - 0.035 * l, At.z - 1);
			glRotated(1, 47, 0, -63);
			glScalef(0.005, 0.005, 0.005);
			glRotated(90, 1, 0, 0);
			model_coin.Draw();
			glPopMatrix();
			e += 1;
			if (e == 20) {
				e = 0;
				l += 1;
			}
		}
	}

}
void level2Part() {
	glPushMatrix();
	glTranslatef(xGirl, jumpGirl, yGirl);
	//glScalef(0.7, 0.7, 0.7);
	model_girl.Draw();
	glPopMatrix();
	for (int i = 0; i < 80; i++) {
		if (!coinsTaken[i]) {
			glPushMatrix();
			if (coinsTranslate[i]) {
				glTranslatef(coins[i][0], translateCoins, coins[i][1]);
			}
			else {
				glTranslatef(coins[i][0], 0.5, coins[i][1]);
			}
			glScalef(0.1, 0.1, 0.1);
			glRotated(90, 1, 0, 0);
			model_coin.Draw();
			glPopMatrix();
		}
	}


	//Draw fire
	for (int i = 1; i < 30; i++) {
		glPushMatrix();
		if (fireTranslate[i]) {
			glTranslatef(fire[i][0], translateFire, fire[i][1]);
			fireTranslate[i] = false;
		}
		else {
			glTranslatef(fire[i][0], 2.5, fire[i][1]);
		}

		glScalef(0.025, 0.025, 0.025);
		model_fire.Draw();
		glPopMatrix();

	}

	//Draw Emerald Goal
	glPushMatrix();
	glTranslatef(0, 0.9, 0);
	//glTranslatef(10, 3, 0);
	glRotated(90, 0, 1, 0);
	glScalef(0.01, 0.012, 0.01);
	model_emerald.Draw();
	glPopMatrix();

}


void skyPart() {
	//sky box
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	//	glTranslated(50, 0, 0);
	//	glRotated(90, 1, 0, 1);
	if (level == 1) {
		glBindTexture(GL_TEXTURE_2D, tex);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex1);
	}
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);

}

void myDisplay(void)
{
	firstPart();
	textPart();
	pumkinPart();
	pumkin2Part();
	RenderGround();

	if (level == 1) {
		level1Part();
	}
	else {
		level2Part();
		pumkin3Part();

	}
	skyPart();
	draw2D();
	//printf("Eye: % f, % f, % f \nAt: %f, %f, %f \nUp: %f, %f, %f \n", Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	float d = 1.1;
	if (!Win && !gameOver) {
		switch (button)
		{
		case 'i':
			if (speedPotion) {
				if (canEnter(xGirl, 0, yGirl - 1.4)) {
					if (firstPerson) {
						Eye.z -= 1.4;
						At.z -= 1.4;
					}
					else {
						Eye.z -= 1.4;
						At.z -= 1.4;
					}
					yGirl -= 1.4;
				}
			}
			else {
				if (canEnter(xGirl, 0, yGirl - 1)) {
					if (firstPerson) {
						Eye.z -= 1;
						At.z -= 1;
					}
					else {
						Eye.z -= 1;
						At.z -= 1;
					}
					yGirl -= 1;
				}
			}
			break;
		case 'j':
			if (speedPotion) {
				if (canEnter(xGirl - 1.4, 0, yGirl)) {
					if (firstPerson) {
						Eye.x -= 1.4;
						At.x -= 1.4;
					}
					else {
						Eye.x -= 1.4;
						At.x -= 1.4;
					}
					xGirl -= 1.4;
				}
			}
			else {
				if (canEnter(xGirl - 1, 0, yGirl)) {
					if (firstPerson) {
						Eye.x -= 1;
						At.x -= 1;
					}
					else {
						Eye.x -= 1;
						At.x -= 1;
					}
					xGirl -= 1;
				}
			}
			break;
		case 'k':
			if (speedPotion) {
				if (canEnter(xGirl, 0, yGirl + 1.4)) {
					if (firstPerson) {
						Eye.z += 1.4;
						At.z += 1.4;
					}
					else {
						Eye.z += 1.4;
						At.z += 1.4;
					}
					yGirl += 1.4;
				}
			}
			else {
				if (canEnter(xGirl, 0, yGirl + 1)) {
					if (firstPerson) {
						Eye.z += 1;
						At.z += 1;
					}
					else {
						Eye.z += 1;
						At.z += 1;
					}
					yGirl += 1;
				}
			}
			break;
		case 'l':
			if (speedPotion) {
				if (canEnter(xGirl + 1.4, 0, yGirl)) {
					if (firstPerson) {
						Eye.x += 1.4;
						At.x += 1.4;
					}
					else {
						Eye.x += 1.4;
						At.x += 1.4;
					}
					xGirl += 1.4;
				}
			}
			else {
				if (canEnter(xGirl + 1, 0, yGirl)) {
					if (firstPerson) {
						Eye.x += 1;
						At.x += 1;
					}
					else {
						Eye.x += 1;
						At.x += 1;
					}
					xGirl += 1;
				}
			}
			break;

			//	case 'o':
				//	if (speedPotion) {
					//	if (!firstPerson) {
						//	rotGirl -= 2;
						//}
					//}
			//		else {
				//		if (!firstPerson) {
					//		rotGirl -= 1;
						//}
					//}
					//break;
			//	case 'u':
				//	if (speedPotion) {
					//	if (!firstPerson) {
						//	rotGirl += 2;
						//}
					//}
					//else {
						//if (!firstPerson) {
							//rotGirl += 1;
						//}
				//	}
					//break;

		case 'w':
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					moveY(d);
					texty += 1.1;
				}
			}
			break;

		case 's':

			if (!firstPerson) {
				if (camerCanEnterDec()) {
					moveY(-d);
					texty -= 1.1;

				}
			}
			break;

		case 'a':
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					moveX(d);
					textx -= 1.1;
					textz += 0.1;
				}
			}
			break;

		case 'd':

			if (!firstPerson) {
				if (camerCanEnterInc()) {
					moveX(-d);
					textx += 1.1;
					textz -= 0.1;
				}
			}

			break;

		case 'q':

			if (!firstPerson) {
				if (camerCanEnterInc()) {
					moveZ(d);
					texty -= 0.5;
				}
			}
			break;

		case 'e':
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					moveZ(-d);
					texty += 0.5;
				}
			}
			break;
		case 'z':
			Xhere += 1;
			break;
		case 'x':
			Xhere -= 1;
		case 'c':
			Yhere += 1;
			break;
		case 'v':
			Yhere -= 1;
		case 'b':
			Zhere += 1;
			break;
		case 'n':
			Zhere -= 1;
			break;
		case 't':
			xDraw += 0.01;
			break;
		case 'y':
			xDraw -= 0.01;
			break;
		case 'h':
			yDraw += 0.01;
			break;
		case 'u':
			yDraw -= 0.01;
			break;
		case 'm':
			printf("%f, %f, %f,  %f, %f, %f, %f, %f, %f \n", xGirl, yGirl, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
			break;
		case 'g':
			if (firstPerson) {
				changedPerson = true;
				moveZ(-1.1);
				moveZ(-1.1);
				moveZ(-1.1);
			}
			firstPerson = !firstPerson;
			if (firstPerson) {
				Eye.x = xGirl + 0.12;
				Eye.y = 1.9;
				Eye.z = yGirl + 2.0;

				At.x = xGirl;
				At.y = 1.9;
				At.z = yGirl + 1.1;

				Up.x = 0.01;
				Up.y = 1;
				Up.z = 0;

				Vector view = (At - Eye).unit();
				Vector temp = view * 1;
				Eye = Eye + temp;
				At = At + temp;

				Vector right = Up.cross(At - Eye).unit();
				temp = right * -0.1;
				Eye = Eye + temp;
				At = At + temp;

			}
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
		}

		glLoadIdentity(); //Clear Model_View Matrix

		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z); //Setup Camera with modified paramters

		GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat light_position1[] = { 0.0f, 0.0f, 0.2f, 0.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);


		glutPostRedisplay(); //Re-draw scene
	}
}
void Special(int key, int x, int y) {

	float a = 0.5;

	if (!Win && !gameOver) {

		switch (key) {
		case GLUT_KEY_UP:
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					rotateX(a);
					texty += 0.075;

				}
			}
			break;

		case GLUT_KEY_DOWN:
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					rotateX(-a);
					texty -= 0.075;

				}
			}
			break;

		case GLUT_KEY_LEFT:
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					rotateY(a);
					textx -= 0.1;
					textz += 0.1;
				}
			}
			break;

		case GLUT_KEY_RIGHT:
			if (!firstPerson) {
				if (camerCanEnterInc()) {
					rotateY(-a);
					textx += 0.1;
					textz -= 0.1;
				}
			}
			break;

		}

		glLoadIdentity(); //Clear Model_View Matrix

		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z); //Setup Camera with modified paramters

		GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glutPostRedisplay(); //Re-draw scene

	}

}

//=======================================================================
// Motion Function
//=======================================================================

//=======================================================================
// Mouse Function
//=======================================================================
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	if (canEnter(xGirl, 0, yGirl - 4)) {
		if (jump == 0 && !firstPerson) {
			jump = 1;
		}
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}



void Timer(int val) {

	if (level == 1) {
		checkDoor();

		if (timepassed % 40 == 0) {
			if (stoneRot >= 20) {
				stoneRot -= 1;
			}
			else {
				stoneRot += 1;
			}
			if (translateTree == 100) {
				translateTree = 1;
				translateCoins = 0.5;
			}
			else {
				translateTree = 100;
				translateCoins = 0.7;
			}
		}
	}
	else {

		if (timepassed % 40 == 0) {
			if (translateTree == 100) {
				translateTree = 1;
				translateCoins = 0.5;
			}
			else {
				translateTree = 100;
				translateCoins = 0.7;
			}
		}


		checkEmerald();

		if (timepassed % 100 == 0) {
			checkPumkin3();
			checkFire();
			if (Xhere <= 1) {
				Xhere += 1;
			}
			else {
				Xhere = 0;
			}
		}
		if (timepassed % 400 == 0) {
			if (translateFire == 2.5) {
				translateFire = 1.5;
			}
			else {
				translateFire = 2.5;
			}
		}

		GLfloat light0_position[] = { -2.0f, 4.0f, 0.0f, 1.0f };

		GLfloat light0_position1[] = { 0.0f, 4.0f, 0.0f, 1.0f };

		GLfloat light0_position2[] = { 2.0f, 4.0f, 0.0f, 1.0f };

		GLfloat lightIntensity1[] = { 0.5, 0.5, 0.5, 1.0f };

		if (level == 2) {
			if (Xhere == 0) {
				glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity1);
				glLightfv(GL_LIGHT2, GL_POSITION, light0_position);
				glLightfv(GL_LIGHT3, GL_POSITION, light0_position);
				glLightfv(GL_LIGHT4, GL_POSITION, light0_position);
				glLightfv(GL_LIGHT5, GL_POSITION, light0_position);
				glLightfv(GL_LIGHT6, GL_POSITION, light0_position);
			}
			else if (Xhere == 1) {
				glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity1);
				glLightfv(GL_LIGHT2, GL_POSITION, light0_position1);
				glLightfv(GL_LIGHT3, GL_POSITION, light0_position1);
				glLightfv(GL_LIGHT4, GL_POSITION, light0_position1);
				glLightfv(GL_LIGHT5, GL_POSITION, light0_position1);
				glLightfv(GL_LIGHT6, GL_POSITION, light0_position1);
			}
			else {
				glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity1);
				glLightfv(GL_LIGHT2, GL_POSITION, light0_position2);
				glLightfv(GL_LIGHT3, GL_POSITION, light0_position2);
				glLightfv(GL_LIGHT4, GL_POSITION, light0_position2);
				glLightfv(GL_LIGHT5, GL_POSITION, light0_position2);
				glLightfv(GL_LIGHT6, GL_POSITION, light0_position2);
			}
		}
	}

	checkCoin();
	checkPumkin();
	checkPumkin2();
	canEnter(xGirl, 0, yGirl);
	if (timepassed % 30 == 0) {
		if (jump == 1) {
			jumpGirl = 1;
			yGirl -= 1;
			jump += 1;
		}
		else if (jump == 2) {
			jumpGirl = 3.5;
			yGirl -= 1;
			jump += 1;
		}
		else if (jump == 3) {
			jumpGirl = 1;
			yGirl -= 1;
			jump += 1;
		}
		else if (jump == 4) {
			jumpGirl = 0;
			yGirl -= 1;
			jump = 0;
		}
	}

	if (timepassed % 30 == 0) {

		if (translatePumkin == 0) {
			translatePumkin = 0.5;
		}
		else {
			translatePumkin = 0;
		}
	}

	if (health >= 0 && !Win && !gameOver) {
		glutPostRedisplay();
		timepassed += 10;
		glutTimerFunc(10, Timer, 0);
	}

	if (sound == 1) {
		PlaySound(TEXT("background.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 2) {
		PlaySound(TEXT("coins.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 3) {
		PlaySound(TEXT("tree.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 4) {
		PlaySound(TEXT("ouch.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 5) {
		PlaySound(TEXT("healthPoints.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 6) {
		PlaySound(TEXT("levelUp.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 7) {
		PlaySound(TEXT("fireResistance.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}
	else if (sound == 8) {
		PlaySound(TEXT("SpeedUp.wav"), NULL, SND_FILENAME | SND_ASYNC);
		sound = 0;
	}


}
//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand((unsigned)time(0)); //seed include c time
	for (int i = 0; i < 80; i++) {
		int x = rand() % 201 - 100;
		int z = rand() % 201 - 100;
		coins[i][0] = x;
		coins[i][1] = z;
	}
	for (int i = 0; i < 80; i++) {
		int x = rand() % 201 - 100;
		int z = rand() % 201 - 100;
		rocks[i][0] = x;
		rocks[i][1] = z;
		stoneTranslate[i] = false;
	}
	for (int i = 0; i < 40; i++) {
		int x = rand() % 201 - 100;
		int z = rand() % 201 - 100;
		trees[i][0] = x;
		trees[i][1] = z;
		treeTranslate[i] = false;
	}

	for (int i = 0; i < 30; i++) {
		int x = rand() % 201 - 100;
		int z = rand() % 201 - 100;
		fire[i][0] = x;
		fire[i][1] = z;
	}

	for (int i = 0; i < 30; i++) {
		fireTranslate[i] = true;
	}

	for (int i = 0; i < 80; i++) {
		coinsTaken[i] = false;
		coinsTranslate[i] = false;
	}
	
	moveZ(-1.1);
	moveZ(-1.1);
	moveZ(-1.1);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	//	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);
	//	glutSpecialFunc(Special);

	glutReshapeFunc(myReshape);

	myInit();

	for (int i = 0; i < 80; i++) {
		coinsTaken[i] = false;
	}
	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	Timer(0);
	glutMainLoop();
}