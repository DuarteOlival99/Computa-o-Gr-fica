//Autores
//Afonso Marques 21705671
//Duarte Olival  21701018
#include <cmath>
#include <iostream>
#include "getbmp.h"
#include "getbmp.cpp"
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;


struct Grass
{
	int x;
	int y;

	Grass();

	Grass(int a, int b) {

		x = a;
		y = b;

	}
};

// Globals.
static int desenhaSun = 1;
static short aAbrirD = 1, aAbrirB = 1, aConstruirH = 1, aAbrirH = 1, aRodarC = 1, aRodarT = 1, aDisparar = 0;
static float t = 0.0; // Animation parameter.
static int tipoDeVista = 2; // Tipo de vista inicial
static float angleD = 0.0, angleB = 0.0, angleH = 20.0, angleC = 0.0, angleT = 0.0, distanciaCannonB = 0, angleSun = 0; // Rotation.angle angleD=door , angleB=briged , angleH = houseDoor, angleC = Cannon , angleT = Tower
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float lookAt[9] = { -0.0, 2500.0, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
static int animationPeriod = 10, animationPeriodSun = 40; // Time interval between frames.
static int speed = 0;
static int isAnimate = 0, isAnimateTower = 1; // Animated?
static int door = 1, briged = 0, house = 0, cannon = 0, tower = 0, cannonBall = 0;
static int iteracao = 0;
static Grass arrayGrass[5000] = {};
static GLsizei width, height; // OpenGL window size.
static int q = 0;
static int grassDisponivel = 0;
static int water = 200; //Utilizado para a altura da agua do poo
static int alturaCannon = 60, alturaDisparo = 30;
static short ativarSol = 0;
//Variavel utilizada para o click esquerdo (Ser ativado apenas uma vez)
static int click = 0;
static unsigned int texture[9]; // Array of texture indices.
//Luz
static float d = 50.0; // Intensity of sun light.
static float theta = 45; // Angle of the sun with the ground.
static float p = 1.0; // Positional light?
static float m = 0.7; // Global ambient white light intensity.
static float h = 50.0; // Shininess.
static int localViewer = 1; // Local viewpoint?
static float xAngle = 0.0, yAngle = 0.0; // Rotation angles of white light.
static float spotAngle = 10.0; // Spotlight cone half-angle.
static float spotExponent = 2.0; // Spotlight exponent = attenuation.
float matShine[] = { h };
#define ONE_BY_ROOT_THREE 0.57735
// Load external textures.d
void loadExternalTextures()
{
	// Local storage for bmp image data.
	BitMapFile* image[9];

	// Load the texture.
	image[0] = getbmp("./Textures/wood.bmp");
	image[1] = getbmp("./Textures/m.bmp");
	image[2] = getbmp("./Textures/telhado.bmp");
	image[3] = getbmp("./Textures/muralha.bmp");
	image[4] = getbmp("./Textures/grass.bmp");
	image[5] = getbmp("./Textures/csky.bmp");
	image[6] = getbmp("./Textures/hole1.bmp");


	// Activate texture index texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);


	// Activate texture index texture[1]. 
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

	// Activate texture index texture[2]. 
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

	// Activate texture index texture[3]. 
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

	// Bind grass image to texture index[4]. 
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);

	// Bind sky image to texture index[5]
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);

	// Bind buraco image to texture index[6]
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);
	




}

void drawGrass(int x, int y) {
	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);

	glVertex3f(x, y, -400);
	glVertex3f(x + 30, y, -400);

	glVertex3f(x, y, -400);
	glVertex3f(x - 30, y + 20, -400);

	glVertex3f(x, y, -400);
	glVertex3f(x - 30, y - 20, -400);

	glVertex3f(x, y, -400);
	glVertex3f(x + 30, y - 20, 3 - 400);

	glVertex3f(x, y, -400);
	glVertex3f(x + 30, y + 20, -400);

	glVertex3f(x, y, -400);
	glVertex3f(x, y + 30, -400);
	glVertex3f(x, y, -400);
	glVertex3f(x, y - 30, -400);

	glVertex3f(x, y, -400);
	glVertex3f(x + 30, y, -400);
	glVertex3f(x, y, -400);
	glVertex3f(x - 30, y, -400);

	glEnd();
	glPopMatrix();
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && cannon == 1) {
		click++;
		if (click % 2 == 0)
		{
			//cout << "Disparei" << endl;
			aDisparar = 1;
			cannonBall = 1;
			distanciaCannonB = 0;
		}

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		// Store the clicked point in the points array after correcting
		// from event to OpenGL co-ordinates.


		int yCerto = height - y;

		//mapeamento do X
		if (x == 0) {
			x = -1200;
		}
		else if (x == 400) {
			x = 0;
		}
		else if (x == 800) {
			x = 1200;
		}
		else {
			x = -1200 + ((x * 3));
		}
		//mapeamento do Y
		if (yCerto == 0) {
			yCerto = -1200;
		}
		else if (yCerto == 350) {
			yCerto = 0;
		}
		else if (yCerto == 700) {
			yCerto = 1200;
		}
		else {
			yCerto = -1200 + ((yCerto * 3.43));
		}

		if (grassDisponivel == 1) {

			if (q < 5000) {

				if ((x > -1170 && x < -870 && yCerto > -580 && yCerto < 580) || //lado esquerdo
					(x < 1170 && x >  870 && yCerto > -580 && yCerto < 580) || //lado direito
					(x > -590 && x < 590 && yCerto < -890 && yCerto > -1170) || // lado de baixo
					(x > -1170 && x < -1020 && yCerto < 1170 && yCerto > 580) ||  //canto supperior lado esquerdo esquerdo
					(x > -1170 && x < -625 && yCerto < 1170 && yCerto > 1000) || //canto supperior lado esquerdo cima
					(x > -1170 && x < -1020 && yCerto > -1170 && yCerto < -580) ||//canto inferior lado esquerdo
					(x > -1170 && x < -625 && yCerto > -1170 && yCerto < -1000) || //canto supperior lado esquerdo baixo
					(x < 1170 && x > 1020 && yCerto < 1170 && yCerto > 580) ||	 //canto superior lado direito
					(x < 1170 && x > 625 && yCerto < 1170 && yCerto > 1000) || // canto superior lado direito cima
					(x < 1170 && x > 1020 && yCerto > -1170 && yCerto < -580) || //canto inferior lado direito
					(x < 1170 && x > 625 && yCerto > -1170 && yCerto < -1000)// canto superior lado direito baixo
					) {



					Grass a(x, yCerto);
					arrayGrass[q] = a;
					//cout << arrayGrass[q].x << " , " << arrayGrass[q].y << endl;
					q++;


				}
			}
			else {
				cout << "impossivel acrescentar mais relva!" << endl;
			}

		}



	}


	glPopMatrix();
}

void viewOriginal() {
	Xangle = 0.0;
	Yangle = 0.0;
	Zangle = -360.0;
}
void viewType() {
	if (tipoDeVista == 1) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, -3000.0, 3000.0);
		gluLookAt(lookAt[0], lookAt[1], lookAt[2], lookAt[3], lookAt[4], lookAt[5], lookAt[6], lookAt[7], lookAt[8]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else if (tipoDeVista == 2) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60, 1, 1, 7000);
		gluLookAt(lookAt[0], lookAt[1], lookAt[2], lookAt[3], lookAt[4], lookAt[5], lookAt[6], lookAt[7], lookAt[8]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();



	}

}
void drawSky() {
	glPushMatrix();

	glLoadIdentity();
	//gluLookAt(0.0, 10.0, 15.0 , 0.0, 10.0, 0, 0.0, 1.0, 0.0);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// Map the sky texture onto a rectangle parallel to the xy-plane. Parte de tras ok
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0); glVertex3f(3000.0, -3000.0, -30000.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-3000.0, -3000.0, -30000.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-3000.0, -3000.0, 30000.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(3000.0, -3000.0, 30000.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// Map the sky texture onto a rectangle parallel to the xy-plane.
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(3000.0, 3000.0, -30000.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(3000.0, 3000.0, 30000.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-3000.0, 3000.0, 30000.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-3000.0, 3000.0, -30000.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// Map the sky texture onto a rectangle parallel to the xy-plane.
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0); glVertex3f(3000.0, 3000.0, 30000.0);
	glTexCoord2f(0.0, 1.0);	glVertex3f(3000.0, 3000.0, -30000.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(3000.0, -3000.0, -30000.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(3000.0, -3000.0, 30000.0);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// Map the sky texture onto a rectangle parallel to the xy-plane.
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3000.0, 3000.0, 30000.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-3000.0, -3000.0, 30000.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-3000.0, -3000.0, -30000.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3000.0, 3000.0, -30000.0);


	// Map the grass texture onto a rectangle along the xz-plane.

	glEnd();
	glPopMatrix();
}
void drawGround() {
	glPushMatrix();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//gluLookAt(0.0, 10.0, 15.0 , 0.0, 10.0, 0, 0.0, 1.0, 0.0);

	// Map the grass texture onto a rectangle along the xz-plane.
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3000, -3000.0, -410);
	glTexCoord2f(16.0, 0.0);  glVertex3f(3000.0, -3000.0, -410);
	glTexCoord2f(16.0, 16.0); glVertex3f(3000.0, 3000.0, -410);
	glTexCoord2f(0.0, 16.0); glVertex3f(-3000, 3000.0, -410);
	glEnd();
	glPopMatrix();
}
void sunInteraction() {


	animationPeriodSun = 40 + speed / 5;
	angleSun += 1.0;
	if (angleSun > 60 && angleSun < 190)
	{
		desenhaSun = 0;

	}
	else
	{
		desenhaSun = 1;
		if (angleSun >= 190 && angleSun <= 300 && ativarSol == 1)
		{
			if (theta < 180.0) theta += 1.0;
		}
		else if ((angleSun > 300 || angleSun < 60) && ativarSol == 1) {
			if (theta > 0.0) theta -= 1.0;
		}

	}
	if (angleSun == 360)
	{
		angleSun = 0;
	}
	//cout << angleSun << endl;
}
void doorInteraction() {
	if (door == 1) {
		animationPeriod = 10 + speed / 5;
		if (isAnimate && aAbrirD == 1)
		{
			angleD += 1.0;
			if (angleD == 90.0) {
				//angleD -= 45.0;
				isAnimate = 0;
				aAbrirD = 0;
			}
		}
		if (isAnimate && aAbrirD != 1)
		{
			angleD -= 1.0;
			if (angleD == 0.0) {
				//angleD -= 45.0;
				isAnimate = 0;
				aAbrirD = 1;
			}
		}
	}
}
void brigedInteraction() {
	if (briged == 1) {
		animationPeriod = 40 + speed;
		if (isAnimate && aAbrirB == 1)
		{
			angleB += 1.0;
			if (angleB == 60.0) {
				isAnimate = 0;
				aAbrirB = 0;
			}
		}
		if (isAnimate && aAbrirB != 1)
		{
			angleB -= 1.0;
			if (angleB == 0.0) {

				isAnimate = 0;
				aAbrirB = 1;
			}
		}
	}
}
void houseInteraction(int value) {
	if (house == 1) {
		if (value)
		{
			if (iteracao >= 20)
			{
				animationPeriod = 20 + speed / 5;
			}
			else
			{

				animationPeriod = 350 + speed;
			}
			if (isAnimate)
			{
				//cout << iteracao << endl;
				iteracao++;
			}

			if (isAnimate && aAbrirH == 1)
			{
				angleH -= 1.0;
				if (angleH == -90.0) {
					isAnimate = 0;
					aAbrirH = 0;
				}
			}
			if (isAnimate && aAbrirH != 1)
			{
				angleH += 1.0;
				if (angleH == 0.0) {

					isAnimate = 0;
					aAbrirH = 1;
				}
			}

		}
	}
}
void top_menu_ortho(int id)
{
	tipoDeVista = 1;
	grassDisponivel = 0;
	viewOriginal();
	if (id == 1) {
		lookAt[0] = 0.0, lookAt[1] = 1.0, lookAt[2] = 1.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	if (id == 2) {
		lookAt[0] = 10.0, lookAt[1] = 0.0, lookAt[2] = 10.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	if (id == 3) {
		lookAt[0] = 0.0, lookAt[1] = 0.0, lookAt[2] = 10.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 1.0, lookAt[8] = 0.0;
	}
	viewType();
	glutPostRedisplay();
}


void cannonInteraction(int value) {

	if (cannon == 1) {

		if (value)
		{
			cout << angleC << endl;
			//Roda o canhão
			animationPeriod = 10 + speed / 5;
			if (isAnimate && aRodarC == 1)
			{
				angleC += 1.0;
				if (angleC == 90.0) {

					//angleD -= 45.0;
					//isAnimateTower = 1;
					isAnimate = 0;
					aRodarC = 0;
					cannonBall = 1;
				}
				if (angleC == 360)
				{
					angleC = 1;
				}
			}
			if (isAnimate && aRodarC != 1)
			{
				if (angleC > 0)
				{
					angleC -= 1.0;
				}

				if (angleC <= 0.0) {
					//angleD -= 45.0;
					isAnimateTower = 1;
					isAnimate = 1;
					aRodarC = 0;
				}
				if (angleC == 360)
				{
					angleC = 1;
				}
			}
		}
	}
}
void towerInteraction() {
	if (tower == 1) {
		animationPeriod = 10 + speed / 5;
		if (isAnimate && aRodarT == 1 && isAnimateTower == 1)
		{

			angleT -= 1.0;
			if (angleT == -180.0) {
				//angleD -= 45.0;
				//isAnimate = 0;
				isAnimateTower = 0;
				cannon = 1;
				aRodarT = 0;
			}
		}
		if (isAnimate && aRodarT != 1 && isAnimateTower == 1)
		{

			angleT += 1.0;
			if (angleT == 0.0) {
				//angleD -= 45.0;
				//isAnimate = 0;
				isAnimateTower = 0;
				aRodarT = 1;
				cannon = 0;
			}
		}
	}
}
void cannonBallInteraction(int value) {
	if (cannonBall == 1) {
		if (value)
		{

			animationPeriod = 3 + speed / 5;

			if (aDisparar == 1)
			{
				if (distanciaCannonB <= 2500) {
					distanciaCannonB += 10;
				}

				if (distanciaCannonB > 2500) {
					isAnimate = 0;
					//distanciaCannonB = 0;
					aDisparar = 0;
				}
			}


		}
	}

}
// Timer function.
void animate(int value)
{

	towerInteraction();
	cannonInteraction(value);
	cannonBallInteraction(value);
	houseInteraction(value);
	doorInteraction();// caso a variavel door esteja ativa (Portas)
	brigedInteraction(); // caso a variavel briged esteja ativa (Ponte)
	glutTimerFunc(animationPeriod, animate, 1);

	glutPostRedisplay();
}

void animateSun(int value)
{
	sunInteraction();
	glutTimerFunc(animationPeriodSun, animateSun, 1);

	glutPostRedisplay();

}
//desenha os circulos
void drawCircle(float xx, float yy, float zz, float pI, float pF, float radius)// Radius of circle.)
{
	glPushMatrix();

	glTranslatef(xx, yy, zz);

	//glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	float angle = PI * pI;
	float angle_stepsize = 0.1;
	float x = xx;
	float y = yy;
	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, zz);
		angle = angle + angle_stepsize;
	}
	glEnd();
	glPopMatrix();

}


void drawPiramide(int x, int y, int z, int tamanhox, int tamanhoz, float tX, float tY, int index) {
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);

	// Command to push the hemisphere, which is drawn centered at the origin, 
	// into the viewing frustum.

	glTranslatef(x, y, z);

	glScalef(tamanhox, tamanhox, tamanhoz);
	glBindTexture(GL_TEXTURE_2D, texture[index]);

	const float size = 0.5;
	glBegin(GL_POLYGON);

	//face de baixo
	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX, tY), glVertex3f(-size, size, -size);
	glTexCoord2f(0.0, tY), glVertex3f(size, size, -size);

	glEnd();

	//face da esquerda
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);
	glTexCoord2f(tX, 0.0), glVertex3f(size, -size, -size);


	glEnd();
	//face da frente
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, -size);

	glTexCoord2f(tX, 0.0), glVertex3f(-size, size, -size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);

	glEnd();
	//face da direita
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0), glVertex3f(-size, size, -size);

	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);

	glEnd();
	//face de tras
	glBegin(GL_POLYGON);

	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX, tY / 2), glVertex3f(0, 0, size);

	glEnd();

	glPopMatrix();
}

//desenha triangulos
void drawTriangle(int x, int y, int z, int tamanhox, int tamanhoy, int tamanhoz, int index) {
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);

	// Command to push the hemisphere, which is drawn centered at the origin, 
	// into the viewing frustum.

	glTranslatef(x, y, z);

	glScalef(tamanhox, tamanhoy, tamanhoz);
	glBindTexture(GL_TEXTURE_2D, texture[index]);


	const float size = 0.5;

	//face de cima
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0), glVertex3f(0, -size, size);
	glTexCoord2f(1, 0.0), glVertex3f(size, size, size);
	glTexCoord2f(1, 1), glVertex3f(-size, size, size);
	glEnd();

	// face de baixo
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0), glVertex3f(0, -size, -size);
	glTexCoord2f(1, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(1, 1), glVertex3f(-size, size, -size);


	glEnd();

	//aresta da direita
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, size);
	glTexCoord2f(1, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(1, 1), glVertex3f(0, -size, size);
	glTexCoord2f(0.0, 1), glVertex3f(0, -size, -size);
	//aresta da esquerda
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0), glVertex3f(-size, size, size);
	glTexCoord2f(1, 0.0), glVertex3f(-size, size, -size);
	glTexCoord2f(1, 1), glVertex3f(0, -size, size);
	glTexCoord2f(0.0, 1), glVertex3f(0, -size, -size);

	glEnd();
	glPopMatrix();




}
//desenha cubos
void drawCube(int x, int y, int z, int  tamanhox, int tamanhoy, int tamanhoz, float rotate, float rotatex, float rotatey, float rotatez, float tX, float tY, int index) {
	//glColor3f(0.0, 0.0, 0.0);


	// Modeling transformations.
	//glLoadIdentity();

	// Command to push the hemisphere, which is drawn centered at the origin, 
	// into the viewing frustum.
	glPushMatrix();

	glRotatef(rotate, rotatex, rotatey, rotatez);
	glTranslatef(x, y, z);
	glScalef(tamanhox, tamanhoy, tamanhoz);


	//	glutWireCube(5.0); // Box.

	const float size = 0.5; // Tamanho do quadrado irá ser 1*1*1
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_QUADS);

	//face de Tras ctw
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(-size, size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX, tY), glVertex3f(-size, -size, size);
	glTexCoord2f(0.0, tY), glVertex3f(-size, size, size);

	glEnd();

	glBegin(GL_QUADS);
	//face da frente ctw ok
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(tX, tY), glVertex3f(size, size, size);
	glTexCoord2f(0.0, tY), glVertex3f(size, -size, size);
	glEnd();
	glBegin(GL_QUADS);

	//face da Direita cw ok
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, size);
	glTexCoord2f(tX, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(tX, tY), glVertex3f(-size, size, -size);
	glTexCoord2f(0.0, tY), glVertex3f(-size, size, size);

	glEnd();

	//face da esquerda nao e preciso desinhar pois as outras faces completam esta
	glBegin(GL_QUADS);

	//face de esquerda ctw ok
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, size);
	glTexCoord2f(tX, tY), glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0, tY), glVertex3f(size, -size, -size);

	glEnd();


	glBegin(GL_QUADS);

	//face de Cima ctW ok
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(-size, -size, size);
	glTexCoord2f(tX, 0.0), glVertex3f(size, -size, size);
	glTexCoord2f(tX, tY), glVertex3f(size, size, size);
	glTexCoord2f(0.0, tY), glVertex3f(-size, size, size);

	glEnd();

	glBegin(GL_QUADS);

	//face de baixo ctw ok
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX, tY), glVertex3f(-size, size, -size);
	glTexCoord2f(0.0, tY), glVertex3f(size, size, -size);

	glEnd();




	glPopMatrix();




}

//desenha os circulos
void drawSCircle(float xx, float yy, float zz, float pI, float pF, float angle_stepsize, float radius, int index)// Radius of circle.)
{
	glPushMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_LINE_LOOP);
	float angle_stepsizee = (PI * pF) / angle_stepsize;
	float angle = PI * pI;
	float x = xx;
	float y = yy;
	while (angle < pF * PI) {

		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(0.0, 0.0), glVertex3f(x, y, zz);
		angle = angle + angle_stepsizee;
	}



	glEnd();
	glPopMatrix();

}


void drawLinesCircleOfCone(float xx, float yy, float zz, float pI, float pF, float qtd_lines, float radius, int index) {

	glPushMatrix();

	float x = xx;
	float y = yy;


	glColor3f(0.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_LINES);
	qtd_lines = (pF * PI) / qtd_lines;
	float angle = pI * PI;
	while (angle < pF * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(0.0, 0.0), glVertex3f(0, 0, 1);
		glTexCoord2f(4, 0.0), glVertex3f(x, y, 0.0);


		angle = angle + qtd_lines;
	}

	glEnd();

	glPopMatrix();

}


//desenha cones
void drawCone(int xx, int yy, int zz, int largura, int altura, float lines, float step_size, float pI, float pF, float radius, int index) {
	glPushMatrix();

	glTranslatef(xx, yy, zz);

	glScalef(largura, largura, altura);


	// Draw the tube 
	drawLinesCircleOfCone(xx, yy, zz, pI, pF, lines, radius, index);

	//Draw the circle on top of cylinder
	drawSCircle(xx, yy, 0, pI, pF, step_size, radius, index);

	glPopMatrix();
}

//desenha as torres
void drawTower(int x, int y, int z, int nrVertices, int alturaz, int nrLinhasVertical) {

	//drawDome(x, y);

	glColor3f(0.0, 0.0, 0.0);
	int squareSize = 120;

	//Quadrado grande
	glPushMatrix();
	glTranslated(0, 0, alturaz / 2 + z);
	drawCube(x, y, 0, squareSize, squareSize, alturaz, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	glPopMatrix();
	int vertices = squareSize / 2 + 15;
	int lado = squareSize / 2 + 12;

	//quadrados a volta do grande
	//Vertices
	int cuboGrande = 30;
	int cuboPequeno = 25;
	drawCube(x + vertices, y + vertices, z, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - vertices, y - vertices, z, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - vertices, y + vertices, z, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x + vertices, y - vertices, z, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//Direita
	drawCube(x + lado, y + lado / 3, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x + lado, y - lado / 3, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//Baixo
	drawCube(x + lado / 3, y - lado, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - lado / 3, y - lado, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//Esquerda
	drawCube(x - lado, y + lado / 3, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - lado, y - lado / 3, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//Cima
	drawCube(x + lado / 3, y + lado, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - lado / 3, y + lado, z, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(x, y, z);
	//glColor3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
	drawCone(0, 0, 0, 20, 100, 1000, 1000, 0, 2, 2, 2);
	//glutSolidCone(squareSize*0.4, squareSize*0.9, nrVertices, nrLinhasVertical);
	//glColor3f(0.0, 0.0, 0.0);
	//glutWireCone(squareSize*0.4, squareSize*0.9, nrVertices, nrLinhasVertical);

	glPopMatrix();


}

void drawSun(void) {
	if (desenhaSun == 1 && ativarSol == 1) {
		glPushMatrix();
		glColor3f(1, 0.61, 0.16);
		glRotatef(angleSun, 0, 1, 0);
		glTranslatef(1200, 100, 800);
		glutSolidSphere(100, 100, 100);
		glPopMatrix();
	}
}

//desenha as portas da entrada
void drawDoor(void) {
	glPushMatrix();
	glPointSize(2);
	glBegin(GL_POINTS);


	glVertex3f(-105, 400, 0);
	glVertex3f(105, 400, 0);

	//calhas da porta
	//horizontais

	drawCube(0, 400, -400, 423, 50, 20, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	drawCube(0, 400, 0, 423, 50, 20, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	//verticais
	drawCube(200, 400, -230, 423, 50, 50, 90.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0);
	drawCube(200, 400, 230, 423, 50, 50, 90.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0);

	glEnd();
	glPopMatrix();
	glPushMatrix();

	//drawCube(0,0,0, 175, 50, -400, 45.0, 0.0, 0.0, 1.0);//porta da direita
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-105, 400, -200);
	glRotatef(angleD, 0.0, 0.0, 1.0);

	drawCube(0, 0, 0, 210, 50, 400, 0, 0.0, 0.0, 0.0, 1.0, 1.0, 0);//porta da direita
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(105, 400, -200);
	glRotatef(-angleD, 0.0, 0.0, 1.0);
	drawCube(0, 0, 0, 210, 50, 400, 0, 0.0, 0.0, 0.0, 1.0, 1.0, 0);//porta da esquerda
	glPopMatrix();

}
void drawBrigde() {

	glPushMatrix();
	//drawCube(0, 1710, 1, 340, 700, 50, 0, 0, 0, 0, 1.0, 2.0, 7.0);
	glPopMatrix();

	glPushMatrix();

	drawCube(0, 728, -430, 425, 368, 50, 0, 0, 0, 0, 1.0, 1.0, 6.0);

	glPopMatrix();

	drawPiramide(262, 850, -330, 100, 100, 1, 1, 0);
	drawPiramide(-262, 850, -330, 100, 100, 1, 1, 0);
	drawPiramide(262, 575, -330, 100, 100, 1, 1, 0);
	drawPiramide(-262, 575, -330, 100, 100, 1, 1, 0);



	drawCube(0, 475, -425, 425, 100, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);
	drawCube(0, 975, -425, 425, 150, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);


	//Suporte da ponte
	//longe do user
	drawCube(262, 850, -425, 100, 100, 100, 0, 0, 0, 0, 1.0, 1.0, 1.0);
	drawCube(-262, 850, -425, 100, 100, 100, 0, 0, 0, 0, 1.0, 1.0, 1.0);
	//perto do user
	drawCube(262, 575, -425, 100, 100, 100, 0, 0, 0, 0, 1.0, 1.0, 1.0);
	drawCube(-262, 575, -425, 100, 100, 100, 0, 0, 0, 0, 1.0, 1.0, 1.0);
	//ponte
	glTranslatef(0, 528, -425);

	glRotatef(angleB, 1.0, 0.0, 0.0);

	drawCube(0, 184, 0, 425, 368, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
	glPopMatrix();


}
//desenha as muralhas
void drawWalls(void) {
	int cuboGrande = 25;
	int triangluloPequeno = 20;



	//Parede frontal
	glPushMatrix();
	glTranslated(0, 0, -200);
	glColor3f(0.0, 0.0, 0.0);
	drawCube(0, -350, 0, 580, 50, -400, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0);
	for (int count = -250; count <= 250; count += 25) {
		drawTriangle(count, -365, 210, triangluloPequeno, -triangluloPequeno, triangluloPequeno, 3);
	}

	for (int count = -250; count <= 250; count += 25) {
		drawTriangle(count, -335, 210, triangluloPequeno, triangluloPequeno, triangluloPequeno, 3);
	}

	glPopMatrix();
	//Parede Direita
	glPushMatrix();
	glTranslated(0, -75, -200);
	//glRotatef(90, 0, 0, 1);
	glColor3f(0.0, 0.0, 0.0);
	drawCube(0, -350, 0, 430, 50, -400, -90.0, 0.0, 0.0, 1.0, 1.0, 2.0, 1.0);
	glRotatef(-90, 0, 0, 1);
	for (int count = -175; count <= 180; count += 25) {
		drawTriangle(count, -365, 210, triangluloPequeno, -triangluloPequeno, triangluloPequeno, 3);
	}
	glPopMatrix();
	//Parede esquerda
	glPushMatrix();
	glTranslated(0, -75, -200);
	//glRotatef(90, 0, 0, 1);
	glColor3f(0.0, 0.0, 0.0);
	drawCube(0, 350, 0, 430, 50, -400, -90, 0.0, 0.0, 1.0, 1.0, 2.0, 1.0);
	glRotatef(-90, 0, 0, 1);
	for (int count = -175; count <= 180; count += 25) {
		drawTriangle(count, 365, 210, triangluloPequeno, triangluloPequeno, triangluloPequeno, 3);
	}
	glPopMatrix();
	//Cima
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	//drawCube(0, 400, -200, 423, 50, -400, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0);

	//Octaedro da esquerda
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-210, 400, 30);
	glScalef(50, 25, 30);
	//glutWireOctahedron();
	glPopMatrix();

	//Octaedro da direita
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(211, 400, 30);
	glScalef(50, 25, 30);
	//glutWireOctahedron();
	glPopMatrix();
	// Parede ponte
	glPushMatrix();
	glTranslated(0, 0, -25);
	glColor3f(0.0, 0.0, 0.0);
	//Ponte
	drawCube(0, 200, 0, 510, 100, 0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0);
	//Quadrados
	//Parte de baixo
	for (int count = -200; count <= 200; count += 50) {
		drawCube(count, 163, 20, cuboGrande, cuboGrande, 20, 0.0, 0.0, 0.0, 0.0, 32.0, 32.0, 3.0);
	}

	//Parte de cima
	for (int count = -175; count <= 175; count += 50) {
		drawCube(count, 237, 20, cuboGrande, cuboGrande, 20, 0.0, 0.0, 0.0, 0.0, 32.0, 32.0, 3.0);
	}

	//linha cima
	//drawCube(0, 225, 0, 460, 0, -20);
	//linha baixo
	//Parede central lado direito
	glPopMatrix();
	glPushMatrix();
	//glRotatef(33, 0.0, 1.0, 0.0);
	drawCube(115, -80, 165, 315, 100, 20, 33.0, 0.0, 1.0, 0.0, 1.0, 2.0, 1.0);
	glPopMatrix();
	//Parede central lado esquerdo
	glPushMatrix();
	//	glRotatef(-33, 0.0, 1.0, 0.0);
	drawCube(-115, -80, 165, 315, 100, 20, -33.0, 0.0, 1.0, 0.0, 1.0, 2.0, 1.0);
	glPopMatrix();
	glPopMatrix();

	//	drawCurve();//Faz a parte curva da parede


}

//desenha os circulos
void drawCircleWall(float xx, float yy, float zz, float pI, float pF, float radius, int index)// Radius of circle.)
{
	glPushMatrix();

	glTranslatef(xx, yy, zz);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	//glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	float angle = PI * pI;
	float angleInicial = angle;
	float angle_stepsize = 0.1;
	float tX = 0.0;
	float tY = 0.0;
	float x = xx;
	float y = yy;
	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.05 <= 1)
		{
			tX += 0.05;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	while (angle > angleInicial) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(tX, tY), glVertex3f(x, y, -400);
		if (tX - 0.05 >= 0)
		{
			tX -= 0.05;
		}
		angle = angle - angle_stepsize;
	}

	glEnd();
	glPopMatrix();

}




//desenha os circulos do cilindro
void drawSCircle(float xx, float yy, float zz, float pI, float pF, float radius, int index)// Radius of circle.)
{
	glPushMatrix();



	glColor3f(0.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	/*
	glBegin(GL_LINE_STRIP);
	float tX = 0.0;
	float tY = 0.0;
	float angle = PI * pI;
	float angleInicial = angle;
	float angle_stepsize = 0.01;
	float x = xx;
	float y = yy;

	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		glTexCoord2f(1, 1), glVertex3f(x, y, 0.0);
		if(tX + 0.045 < 1)
		{
			tX += 0.045;
		}

		angle = angle + angle_stepsize;
	}

		glEnd();
	glPopMatrix();
	*/

	float tX = 0.0;
	float tY = 0.0;
	float angle = PI * pI;
	float angleInicial = angle;
	float angle_stepsize = 0.1;
	float x = xx;
	float y = yy;
	int xX = 0;
	int yY = 0;
	float angleNovo;

	glBegin(GL_POLYGON);

	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 0.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 0)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 0.25;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 0.5)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 0.25)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 0.5;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 0.75)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 0.5)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 0.75;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 0.75)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 4) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	tX = 1;
	tY = 0;
	glBegin(GL_POLYGON);
	angle = angleNovo;
	while (angle < (pF * PI + 0.1)) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		//	glVertex3f(x, y, zz);
		xX++;
		angle = angle + angle_stepsize;
	}

	angle = angleInicial;
	x = xx;
	y = yy;
	while (yY < xX / 2) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
		glTexCoord2f(tX, tY), glVertex3f(x, y, zz);
		if (tX + 0.01 <= 1.25)
		{
			tX += 0.01;
		}
		angle = angle + angle_stepsize;
	}
	tY = 1;
	angle = angle - angle_stepsize;
	angleNovo = angle;
	while (yY > 0) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY--;
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX - 0.01 >= 1)
		{
			tX -= 0.01;
		}
		angle = angle - angle_stepsize;
	}
	glEnd();
	/*
	while (yY <= xX ) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		yY++;
	//glVertex3f(x, y, zz);
		angle = angle + angle_stepsize;
	}

	*/
	glBegin(GL_POLYGON);
	angle = angleInicial;
	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(tX, tY), glVertex3f(x, y, 1);
		if (tX + 0.05 < 1)
		{
			tX += 0.05;
		}

		angle = angle + angle_stepsize;
	}
	glEnd();

	glBegin(GL_POLYGON);
	angle = angleInicial;
	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glTexCoord2f(tX, tY), glVertex3f(x, y, 0);
		if (tX + 0.05 < 1)
		{
			tX += 0.05;
		}

		angle = angle + angle_stepsize;
	}
	glEnd();

	glPopMatrix();


}
//desenha as linhas a unir 2 os 2 circulos do cilindro 
void drawLinesCircle(float xx, float yy, float zz, float qtd_lines, float pI, float pF, float radius) {

	glPushMatrix();

	float height = 1.0;
	float x = xx;
	float y = yy;


	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	float angle = pI * PI;
	while (angle < pF * PI + 0.1) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + qtd_lines;
	}

	glEnd();

	glPopMatrix();

}
//desenha o cilindro
void drawCylinder(int xx, int yy, int zz, int largura, int altura, float lines, float pI, float pF, float radius)
{
	glPushMatrix();

	glTranslatef(xx, yy, zz);

	glScalef(largura, largura, altura);

	float height = 1.0;


	// Draw the tube 



	//drawLinesCircle(xx, yy, zz, lines, pI, pF, radius);

	//Draw the circle on top of cylinder
	//drawSCircle(xx, yy, height, pI, pF, radius);
	drawSCircle(xx, yy, 0, pI, pF, radius, 0);





	glPopMatrix();


}
//desenha os vertices curvos das torres de fora
void drawVerticeTower(int x, int y, int z, int espaco_torres, float lines, int largura, int altura, float pI, float pF, float radius) {

	glPushMatrix();

	drawCylinder(x + espaco_torres, +y + espaco_torres, z, largura, altura, lines, pI, pF, radius);//rigth up
	drawCube(x + espaco_torres + 50, y + espaco_torres - 130, z + espaco_torres, largura * 2, largura * 3, altura * 2 - 190, 45, 0, 0, 1, 1, 1, 0);

	drawCylinder(-x - espaco_torres, +y + espaco_torres, z, largura, altura, lines, pI + 0.5, pF + 0.5, radius); // left up

	drawCube(-x - espaco_torres - 50, -y - espaco_torres + 130, z + espaco_torres, largura * 2, largura * 3, altura * 2 - 190, -45, 0, 0, 1, 1, 1, 0);

	drawCylinder(-x - espaco_torres, -y - espaco_torres, z, largura, altura, lines, pI + 1.0, pF + 1.0, radius);//left down

	drawCube(-x - espaco_torres - 50, -y - espaco_torres + 130, z + espaco_torres, largura * 2, largura * 3, altura * 2 - 190, 45, 0, 0, 1, 1, 1, 0);

	drawCylinder(+x + espaco_torres, -y - espaco_torres, z, largura, altura, lines, pI + 1.5, pF + 1.5, radius);// rigth down

	drawCube(x + espaco_torres + 50, -y - espaco_torres + 130, z + espaco_torres, largura * 2, largura * 3, altura * 2 - 190, -45, 0, 0, 1, 1, 1, 0);

	glPopMatrix();

}
//desenha o poco
void drawWell(int xx, int yy, int zz, int tamanhox, int tamanhoy, int tamanhoz, float radius) {
	glPushMatrix();
	drawCylinder(-280, 50, -500, 50, 180, 1.0, 0, 2, 0.5);

	for (int count = -250; count < -water; count += 3) {
		for (float i = 20, cor = 1.0; i >= 0; i -= 5, cor -= 0.25) {
			glColor3f(0.0, cor, 1);
			drawCircle(-280, 50, count, 0, 2, i);
		}
	}
	glPopMatrix();

}
//desenha a torre centrar circular
void drawCircularTower(int x, int y, int z, int largura, int altura, float radius) {
	glPushMatrix();

	//drawCylinder(x, y, z, largura, altura, 0, 2, radius);
	drawCylinder(0, 400, 0, 45, altura, 1.0, 0, 2, 1);
	drawCylinder(0, 400, 0, 75, altura, 1.0, 0, 2, 1);
	int squareSize = 120;

	int ladoQuadradoGrande = squareSize / 2;
	int ladoQuadradoPequeno = squareSize / 2 - 19;


	//quadrados a volta do grande
	//Vertices
	int cuboGrande = 30;
	int cuboPequeno = 20;
	drawCube(x, y + ladoQuadradoGrande, z + altura, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);//cima
	drawCube(x, y - ladoQuadradoGrande, z + altura, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);//baixo
	drawCube(x + ladoQuadradoGrande, y, z + altura, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);//direita
	drawCube(x - ladoQuadradoGrande, y, z + altura, cuboGrande, cuboGrande, cuboGrande, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);//esquerda

	//Baixo
	drawCube(x + ladoQuadradoPequeno, y - ladoQuadradoPequeno, z + altura, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - ladoQuadradoPequeno, y - ladoQuadradoPequeno, z + altura, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//Cima
	drawCube(x + ladoQuadradoPequeno, y + ladoQuadradoPequeno, z + altura, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	drawCube(x - ladoQuadradoPequeno, y + ladoQuadradoPequeno, z + altura, cuboPequeno, cuboPequeno, cuboPequeno, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	glPopMatrix();
}
//desenha  2 "bolas" em cima das torres 
void drawBall(int x, int y, int z, int tamanho) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(tamanho, tamanho, tamanho * 0.75);
	glutWireSphere(0.5, 9, 5);
	glPopMatrix();

}
//desenha a parede grande da muralha de fora
void drawBigWall(int x, int y, int z, int tamanhoX, int tamanhoY, int tamanhoZ, float rotate, float rotateX, float rotateY, float rotateZ, float tX, float tY) {
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	int ladoCubo;
	if (tamanhoY < tamanhoX) {
		ladoCubo = tamanhoY / 4;
		for (int count = -tamanhoX / 2 + 15; count < tamanhoX / 2; count += 50) {
			drawCube(count + ladoCubo / 2, y + tamanhoY / 2 - ladoCubo / 2, ladoCubo / 2 + z + tamanhoZ / 2, ladoCubo, ladoCubo, ladoCubo, rotate, rotateX, rotateY, rotateZ, 4, 4, 3.0);
			drawCube(count + ladoCubo / 2, y - tamanhoY / 2 + ladoCubo / 2, ladoCubo / 2 + z + tamanhoZ / 2, ladoCubo, ladoCubo, ladoCubo, rotate, rotateX, rotateY, rotateZ, 4, 4, 3.0);
		}
	}
	else {
		ladoCubo = tamanhoX / 4;
		for (int count = -tamanhoX / 2 + 15; count < tamanhoX / 2; count += 50) {
			drawCube(count + ladoCubo / 2, x + tamanhoX / 2 - ladoCubo / 2, ladoCubo / 2 + z + tamanhoZ / 2, ladoCubo, ladoCubo, ladoCubo, rotate, rotateX, rotateY, rotateZ, 4, 4, 3.0);
			drawCube(x - tamanhoX / 2 + ladoCubo / 2, count + ladoCubo / 2, ladoCubo / 2 + z + tamanhoZ / 2, ladoCubo, ladoCubo, ladoCubo, rotate, rotateX, rotateY, rotateZ, 4, 4, 3.0);
		}
	}
	drawCube(x, y, z, tamanhoX, tamanhoY, tamanhoZ, rotate, rotateX, rotateY, rotateZ, tX, tY, 1.0);


	glPopMatrix();


}
//desenha a torre da muralha de fora
void drawTowerBigWall(int x, int y, int z) {

	glPushMatrix();
	glTranslatef(x, y, 100);
	drawCone(0, 0, 0, 50, 200, 1000, 1000, 0, 2, 2, 2);
	//glutWireCone(100, 200, 3, 1);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(x, y, z);
	//partes curvas
	drawVerticeTower(0, 0, 0, 100, 1.6, 20, 500, 0, 0.5, 1.5);//lado de dentro
	drawVerticeTower(25, 25, 0, 100, 2.2, 35, 500, -0.1, 0.6, 1.9);//lado de fora


	drawBigWall(0, 160, 250, 208, 60, 500, 0, 0, 0, 0, 1, 1);
	drawBigWall(0, -160, 250, 208, 60, 500, 0, 0, 0, 0, 1, 1);
	drawBigWall(0, 160, 250, 208, 60, 500, 90.0, 0, 0, 1.0, 1, 1);
	drawBigWall(0, 160, 250, 208, 60, 500, -90.0, 0, 0, 1.0, 1, 1);

	drawCube(0, 0, 250, 260, 260, 500, 0, 0, 0, 0, 1, 1, 0);


	glPopMatrix();


}
void drawPrisma(int x, int y, int z, int tamanhox, int tamanhoy, int tamanhoz, float tX, float tY, int index) {
	// Modeling transformations.
	glPushMatrix();

	// Command to push the hemisphere, which is drawn centered at the origin, 
	// into the viewing frustum.

	glTranslatef(x, y, z);

	glScalef(tamanhox, tamanhoy, tamanhoz);

	const float size = 0.5;

	//triangulo frente
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX / 2, tY), glVertex3f(size, 0, size);
	glEnd();


	//triangulo tras
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0), glVertex3f(-size, size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX / 2, tY), glVertex3f(-size, 0, size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0), glVertex3f(size, size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, size, -size);
	glTexCoord2f(tX, tY), glVertex3f(size, 0, size);
	glTexCoord2f(0.0, tY), glVertex3f(-size, 0, size);
	glEnd();



	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0), glVertex3f(size, -size, -size);
	glTexCoord2f(tX, 0.0), glVertex3f(-size, -size, -size);
	glTexCoord2f(tX, tY), glVertex3f(size, 0, size);
	glTexCoord2f(0.0, tY), glVertex3f(-size, 0, size);
	glEnd();

	glPopMatrix();

}
void drawCannonBall(void) {
	glPushMatrix();
	if (isAnimateTower != 1 && cannon == 1)
	{

		glTranslatef(cos((PI * angleC) / 180) * distanciaCannonB, -80 + sin((PI * angleC) / 180) * distanciaCannonB, 205 + tan((PI * alturaDisparo) / 180) * distanciaCannonB);

		glRotatef(angleC - 90, 0, 0, 1);
		//drawCircle(0,0,0,0,2,20);
		//bala do Tubo 
		//glTranslatef(0, -40, 100);
		//glRotatef(-alturaDisparo, 1, 0, 0);
		glTranslatef(0, -40, 100);
		glRotatef(-alturaCannon, 1, 0, 0);

		glColor3f(0, 0, 0);
		glutSolidSphere(15, 15, 15);

	}
	glPopMatrix();
}
//desenha as curvas da muralha e seus respetivos octaedros em cima da muralha
void drawCurve(void) {

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	//Esquerda
	drawCircleWall(-210, 260, 0, 0.5, 1, 165, 1);//cima fora
	/*
	drawCircle(-210, 260, 0, 0.5, 1, 115);//cima dentro
	drawCircle(-210, 260, -200, 0.5, 1, 165);//baixo fora
	drawCircle(-210, 260, -200, 0.5, 1, 115);//baixo dentro
	*/
	//Octaedro na curva
	glPushMatrix();
	glTranslatef(-309, 360, 30);
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(50, 25, 30);
	//glutWireOctahedron();
	glPopMatrix();

	//Direita

	drawCircleWall(210, 260, 0, 0, 0.5, 165, 1);//cima fora
	/*	drawCircle(210, 260, -200, 0, 0.5, 165);//baixo fora
	drawCircle(210, 260, 0, 0, 0.5, 115);//cima dentro
	drawCircle(210, 260, -200, 0, 0.5, 115);//baixo dentro
	*/
	//Octaedro na curva
	glPushMatrix();
	glTranslatef(309, 360, 30);
	glRotatef(-45, 0.0, 0.0, 1.0);
	glScalef(50, 25, 30);
	//glutWireOctahedron();
	glPopMatrix();

	glPopMatrix();

}


void drawCannon() {
	if (isAnimateTower != 1 && cannon == 1)
	{

		drawCannonBall();
		glPushMatrix();
		glTranslatef(0, -80, 205);
		glRotatef(angleC - 90, 0, 0, 1);
		glPushMatrix();
		//drawCircle(0,0,0,0,2,20);
		//Tubo da bala
		glTranslatef(0, -40, 100);
		glRotatef(-alturaCannon, 1, 0, 0);
		drawCylinder(0, 0, 0, 15, 120, 0.9, 0.0, 2.0, 1);
		glutWireSphere(15, 10, 5);
		glPopMatrix();
		//Roda do canhão direita
		glPushMatrix();
		glTranslatef(20, 0, 85);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(3, 40, 10, 25);
		//Parte de dentro da roda
		glutSolidTorus(3, 3, 10, 25);
		glPopMatrix();
		//Roda do canhão esquerda
		glPushMatrix();
		glTranslatef(-20, 0, 85);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(3, 40, 10, 25);
		//Parte de dentro da roda
		glutSolidTorus(3, 3, 10, 25);
		glPopMatrix();
		glPopMatrix();
	}


}

void drawHouse() {
	//int x, int y, int z, int tamanhoX, int tamanhoY, int tamanhoZ, float angluRotacao, float ratacaoX, float RotacaoY, float RotacaoZ, int nrCubos



	//casa ->
	//pilares
	glColor3f(0, 0, 0);
	for (int i = 0, max = 0; i < 5; i++, max++) {
		//cima
		if (iteracao > max)
		{
			drawCube(-480, 200, -370 + (50 * i), 50, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
			drawCube(-680, 200, -370 + (50 * i), 50, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
			drawCube(-480, -200, -370 + (50 * i), 50, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
			drawCube(-680, -200, -370 + (50 * i), 50, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
		}

		//baixo
	}

	//paredes
	glColor3f(0, 0, 0);
	for (int i = 0, max = 5; i < 5; i++, max++) {
		//lado esquerdo
		//lado direito
		//cima
		//baixo
		if (iteracao > max)
		{
			drawCube(-480, 0, -370 + (50 * i), 50, 350, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);
			drawCube(-680, 0, -370 + (50 * i), 50, 350, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);
			//drawCube(-580, 200, -370 + (50 * i), 150, 50, 50, 0, 0, 0, 0);
			drawCube(-580, -200, -370 + (50 * i), 150, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);


		}

		//glutTimerFunc(animationPeriod, animate, 2);
		//glutPostRedisplay(); 
	}
	//teto
	glColor3f(0, 0, 0);
	for (int i = 0, max = 10; i < 5; i++, max++) {
		if (iteracao > max) {
			drawCube(-480 - (50 * i), 0, -120, 50, 450, 50, 0, 0, 0, 0, 1.0, 1.0, 1.0);
		}
	}
	glPushMatrix();
	for (int i = 0, max = 15; i < 5; i++, max++) {
		//lado esquerdo
		//lado direito
		//cima
		//baixo
		//Desenha a porta
		if (iteracao > max && iteracao < 20)
		{
			glColor3f(0.0, 1.0, 1.0);
			drawCube(-580, 200, -370 + (50 * i), 150, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
			glColor3f(0.0, 0.0, 0.0);

		}
	}
	glPopMatrix();
	glPushMatrix();
	if (iteracao >= 20)
	{
		//Desenha o telhado
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-580, 0, -70);
		glRotatef(90, 0, 0, 1);

		drawPrisma(0, 0, 0, 450, 250, 50, 0.5, 0.5, 0);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-580, 0, -70);
		glRotatef(270, 0, 0, 1);
		//glRotatef(0, 1, 0, 0);
		drawPrisma(0, 0, 0, 450, 250, 50, 0.5, 0.5, 0);
		glPopMatrix();

		//Abre a porta
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(-530, 200, -370);
		glRotatef(angleH, 0.0, 0.0, 1 + .0);

		for (int count = 0; count < 5; count++) {
			drawCube(-50, 0, 0 + (50 * count), 150, 50, 50, 0, 0, 0, 0, 1.0, 1.0, 0.0);
		}
		//Ponto de rotação da porta
		glColor3f(0.0, 0.0, 0.0);
		glPointSize(2);
		glBegin(GL_POINTS);
		glVertex3f(0, 0, 225);
		//glVertex3f(0, 0, -25);

	}

	glPopMatrix();

	glPushMatrix();


	glEnd();
	glPopMatrix();



	// <- casa
}

void drawScene(void)
{
	// Light property vectors that change during runtime.
	float lightDifAndSpec[] = { d, d, d, 1.0 };
	float lightPos[] = { cos((PI / 180.0) * theta), sin((PI / 180.0) * theta), tan((PI / 180.0) * theta), 0.0 };

	float lightPos1[] = { 0, 0, 0, 1.0 }; // Spotlight position.
	float spotDirection[] = { -1.0, 0.0, 0.0 }; // Spotlight direction.  

	// Light properties geral.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	// Light properties Torres
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spotExponent);


	// PT: Limpa os desenhos existentes, trocando a matriz actual pela matriz identidade.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	drawSky();
	drawGround();
	//glDisable(GL_LIGHTING);
 //chamar as funcoes aqui________________________________________________
	// Commands to turn the Castle.
	//glRotatef(300, 1.0, 0.0, 0.0);
	//glEnable(GL_LIGHTING);
	drawCannon();
	drawHouse();
	/*
	// Loop through the points array drawing each point.
	for (int i = 0; i < q; i++) {

		drawGrass(arrayGrass[i].x, arrayGrass[i].y);

	}
	*/
	drawWell(-280, 50, -500, 50, 50, 180, 0.5);
	drawSun();
	glColor3f(0.7, 0.7, 0.7);

	//Torres
	drawTower(350, -350, 0, 6, -400, 2);//torre direita baixa 
	drawTower(315, 200, 0, 0, -400, 0);//torre direita cima 
	drawTower(-350, -350, 0, 6, -400, 2);//torre esquerda baixa 
	drawTower(-315, 200, 0, 0, -400, 0);//torre esquerda cima 



	drawCircularTower(0, 400, 0, 100, 90, 0.6);//torre circular do centro da entrada em cima da muralha
	glPushMatrix();
	glTranslatef(0, -80, 200);
	glRotatef(angleT, 1, 1, 0);
	drawTower(0, 0, 0, 20, -50, 2);//Central
	glPopMatrix();
	//Desenha walls
	drawWalls();
	//Desenha o poço;
	drawCurve();
	//desenha a porta

	drawDoor();

	//desenhar as muralhas a volta
	drawBigWall(0, -800, -200, 1220, 105, 400, 90, 0, 0, 1, 1, 4);
	drawBigWall(0, -800, -200, 1220, 105, 400, -90, 0, 0, 1, 1, 4);
	drawBigWall(0, -800, -200, 1220, 105, 400, 0, 0, 0, 0, 1, 4);
	//drawBigWall(0, 800, -200, 1214, 80, 400, 0, 0, 0, 0);
	//drawCube(690,80,-200,390,80,400,40,0,0,1);
	//paredes diagonais
	drawCube(690, 80, -200, 430, 80, 400, 40, 0, 0, 1, 1.0, 1.0, 1.0);
	drawCube(-690, 80, -200, 430, 80, 400, -40, 0, 0, 1, 1.0, 1.0, 1.0);


	drawTowerBigWall(800, 800, -400);
	drawTowerBigWall(-800, 800, -400);

	drawTowerBigWall(800, -800, -400);
	drawTowerBigWall(-800, -800, -400);

	drawBrigde();
	glEnd();

	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, -600.0);
	glVertex3f(d * cos((PI / 180.0) * theta), d * sin((PI / 180.0) * theta), -6.0);
	glEnd();

	glEnable(GL_LIGHTING); // Restore lighting.
	glutSwapBuffers();
	//drawTowerBigWall(-800, 800, -400);
	//drawVerticeTower(25, 25, 0, 100, 2.2, 200, 500, -0.1, 0.6, 1.9);

	//drawCone(200, 200, 0, 70, 400,1000, 1000, 0, 2, 2,2);


}
// Initialization routine.
void setup(void)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Light property vectors that are constant.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	 // Material property vectors.
	float matAmbAndDif[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };
	//Luz 1
	float lightDifAndSpec1[] = { 1.0, 1.0, 1.0, 1.0 };

	//Luz 1
	// Light0 properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);


	// Material properties.
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

	// Create texture index array and load external textures.
	glGenTextures(9, texture);
	loadExternalTextures();

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);

	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

//	glEnable(GL_LIGHT1); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint
	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	viewType();
	//gluPerspective(60, 1, 0, 1000);
	//gluLookAt(0.0, 0.0, 2500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		if (isAnimate) {
			isAnimate = 0;
		}
		else {
			isAnimate = 1;
		}
		glutPostRedisplay();
		break;
	case '+':
		if (water > 160)
		{
			water -= 5;
		}
		d++;
		glutPostRedisplay();
		break;
	case 'w':
		if (cannon == 1 && alturaCannon > 40)
		{
			alturaDisparo += 5;
			alturaCannon -= 5;

		}
		glutPostRedisplay();
		break;
	case 's':
		if (cannon == 1 && alturaCannon < 110)
		{
			alturaDisparo -= 5;
			alturaCannon += 5;

		}
		glutPostRedisplay();
		break;
	case 'a':
		if (cannon == 1 && (angleC + 5 <= 360))
		{
			angleC += 5;
		}
		glutPostRedisplay();
		break;
	case 'd':
		if (cannon == 1 && (angleC - 5 >= 0))
		{

			angleC -= 5;
		}

		glutPostRedisplay();
		break;
	case '-':
		if (water < 250)
		{
			water += 5;
		}
		d--;
		glutPostRedisplay();
		break;
	case 'r':
		angleD -= angleD;//Door
		angleB -= angleB;//Brigde
		angleH -= angleH - 20;//House
		angleC = 0;//Canhão
		angleT -= angleT;//Torre
		isAnimate = 0;
		cannon = 0;
		//tower = 0;
		isAnimateTower = 1;
		aAbrirB = 1;
		aAbrirD = 1;
		aAbrirH = 1;
		aConstruirH = 1;
		aRodarT = 1;
		aRodarC = 1;
		aDisparar = 0;
		iteracao = 0;
		speed = 0;
		water = 200;

		break;
	case 't':
		if (isAnimateTower)
		{
			isAnimateTower = 0;
		}
		else {
			isAnimateTower = 1;
			angleC = 0;
			aRodarC = 1;
		}
		break;
	case 'p':
		if (ativarSol)
		{
			ativarSol = 0;
		}
		else {
			ativarSol = 1;
		}
		break;
	case 'h':
		if (h > 0.0) h -= 1.0;
		glutPostRedisplay();
		break;
	case 'H':
		if (h < 128.0) h += 1.0;
		glutPostRedisplay();
		break;
	case 'm':
		if (m > 0.0) m -= 0.05;
		glutPostRedisplay();
		break;
	case 'M':
		if (m < 1.0) m += 0.05;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	case 'X':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'x':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN)
	{
		if (d > 0.0) d -= 0.05;

	}
	if (key == GLUT_KEY_UP)
	{
		if (d < 1.0) d += 0.05;

	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (theta > 0.0) theta -= 1.0;

	}
	if (key == GLUT_KEY_LEFT)
	{
		if (theta < 180.0) theta += 1.0;

	}
	glutPostRedisplay();
}


// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	/*
	cout << "Interaction:" << endl;
	cout << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
	*/
	cout << "____________Segunda entrega____________" << endl;
	cout << "Teclas passiveis de utilizar" << endl;
	cout << "[a,w,s,d]: Controlar o canhao" << endl;
	cout << "[p]: Ativar o sol" << endl;
	cout << "[+,-]: Aumentar/ Diminuir agua do poco" << endl;
	cout << "[KEY_UP,KEY_DOWN]: Aumentar/ Diminuir velociade das animacoes" << endl;

}
// The top menu callback function.
//meta 2
// MENU 

void top_menu(int id)
{
	if (id == 1) exit(0);
}
//Aqui
void top_menu_ortho_top(int id)
{
	tipoDeVista = 1;
	grassDisponivel = 0;
	viewOriginal();
	if (id == 1) {
		grassDisponivel = 1;
		lookAt[0] = 0.0, lookAt[1] = 0.0, lookAt[2] = 10.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 1.0, lookAt[8] = 0.0;
	}
	if (id == 2) {
		gluLookAt(lookAt[0] = 5, lookAt[1] = 20, lookAt[2] = 30, lookAt[3] = 0, lookAt[4] = 0, lookAt[5] = 0, lookAt[6] = 0, lookAt[7] = 0, lookAt[8] = 1);

	}
	if (id == 3) {

		gluLookAt(lookAt[0] = -5, lookAt[1] = 20, lookAt[2] = 30, lookAt[3] = 0, lookAt[4] = 0, lookAt[5] = 0, lookAt[6] = 0, lookAt[7] = 0, lookAt[8] = 1);

	}
	viewType();
	glutPostRedisplay();
}
void top_menu_perspetive(int id)
{
	tipoDeVista = 2;
	grassDisponivel = 0;
	viewOriginal();
	if (id == 1) {
		lookAt[0] = 0.0, lookAt[1] = 3000.0, lookAt[2] = 0.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	if (id == 2) {
		lookAt[0] = 3000.0, lookAt[1] = 0.0, lookAt[2] = 0.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	if (id == 3) {
		lookAt[0] = 0.0, lookAt[1] = 0.0, lookAt[2] = 2500.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 1.0, lookAt[8] = 0.0;
	}
	viewType();
	glutPostRedisplay();
}
void top_menu_perspetive_top(int id)
{
	tipoDeVista = 2;
	viewOriginal();
	grassDisponivel = 0;
	if (id == 1) {

		lookAt[0] = 0.0, lookAt[1] = 0.0, lookAt[2] = 2500.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 1.0, lookAt[8] = 0.0;
	}
	if (id == 2) {
		lookAt[0] = 1000.0, lookAt[1] = 1800.0, lookAt[2] = 2500.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	if (id == 3) {
		lookAt[0] = -1000.0, lookAt[1] = 1800.0, lookAt[2] = 2500.0, lookAt[3] = 0.0, lookAt[4] = 0.0, lookAt[5] = 0.0, lookAt[6] = 0.0, lookAt[7] = 0.0, lookAt[8] = 1.0;
	}
	viewType();
	glutPostRedisplay();
}
void top_menu_animacoes(int id) {

	if (id == 1) {
		door = 1;
		briged = 0;
		house = 0;
		tower = 0;
		cannon = 0;
	}
	if (id == 2) {
		door = 0;
		briged = 1;
		house = 0;
		tower = 0;
		cannon = 0;
	}
	if (id == 3) {
		door = 0;
		briged = 0;
		house = 1;
		tower = 0;
		cannon = 0;
		iteracao = 0;
	}
	if (id == 4) {
		door = 0;
		briged = 0;
		house = 0;
		tower = 1;
		//Coloca em ortho lateral
		top_menu_ortho(2);
		//iteracao = 0;
	}
	isAnimate = 0;
	glutPostRedisplay();
}

void makeMenu(void)
{
	int menu_ortho_top;
	menu_ortho_top = glutCreateMenu(top_menu_ortho_top);
	glutAddMenuEntry("Centro", 1);
	glutAddMenuEntry("Esquerda", 2);
	glutAddMenuEntry("Direita", 3);

	int menu_perspetive_top;
	menu_perspetive_top = glutCreateMenu(top_menu_perspetive_top);
	glutAddMenuEntry("Centro", 1);
	glutAddMenuEntry("Esquerda", 2);
	glutAddMenuEntry("Direita", 3);

	int menu_ortho;
	menu_ortho = glutCreateMenu(top_menu_ortho);
	glutAddMenuEntry("Frente", 1);
	glutAddMenuEntry("Lateral", 2);
	glutAddSubMenu("Topo", menu_ortho_top);

	int menu_perspetive;
	menu_perspetive = glutCreateMenu(top_menu_perspetive);
	glutAddMenuEntry("Frente", 1);
	glutAddMenuEntry("Lateral", 2);
	glutAddSubMenu("Topo", menu_perspetive_top);

	int menu_animacoes;
	menu_animacoes = glutCreateMenu(top_menu_animacoes);
	glutAddMenuEntry("Door", 1);
	glutAddMenuEntry("Briged", 2);
	glutAddMenuEntry("House", 3);
	glutAddMenuEntry("Cannon", 4);



	// The top menu is created: its callback function is registered and menu entries,
	// including a submenu, added.
	glutCreateMenu(top_menu);
	//glutAddSubMenu("glOrtho", menu_ortho);
	glutAddSubMenu("glOrtho", menu_ortho);
	glutAddSubMenu("gluPerspetive", menu_perspetive);
	glutAddSubMenu("Animações", menu_animacoes);
	glutAddMenuEntry("Quit", 1);



	// The menu is attached to a mouse button.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 700);
	glutInitWindowPosition(350, 0);
	glutCreateWindow("Castle.cpp");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(1, animate, 10);
	glutTimerFunc(1, animateSun, 10);
	makeMenu();

	// Register the mouse callback function. 
	glutMouseFunc(mouseControl);

	glutMainLoop();


	return 0;
}

Grass::Grass()
{
}