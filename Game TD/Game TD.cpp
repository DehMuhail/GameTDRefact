
#define _USE_MATH_DEFINES

#include  <stdlib.h>
#include  <math.h>
#include  <time.h>
#include  <string.h>
#include "gl/glut.h"
#include "matrix.h"
#include "Shapes.h"
#include <thread>
#include <chrono>
//#include <list>

#pragma comment(lib, "freeglut.lib")
long long counterT = 0; 
const int w = 1280;  
const int h = 720; 
int xx, yy;
int ButtonStatus = 0; // 
int  status = 0; // 0 - stop, 1 - start, 2- game over. 3- skins 
int Tstatus = 2; 
int Fstatus = 1;
int currentTower = -1;
float textures[4][2] =
{
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
};
matrix<int>* pRoad;
const int M = 32, N = 18;
double* a;


void displayText(float x, float y, const char* s);
GLuint LoadTexture(GLuint tex, const char* filename, int width, int height)
{
	unsigned char* data;
	unsigned char R, G, B;
	FILE* file;

	fopen_s(&file, filename, "rb");

	if (file == NULL)return 0;
	
	data = (unsigned char*)malloc(width * height * 3);

	BITMAPFILEHEADER header, * p;
	p = &header;
	fseek(file, 0, 0);
	fread(p, sizeof(header), 1, file);
	
	fseek(file, header.bfOffBits, 0);

	
	fread(data, width * height * 3, 1, file);

	fclose(file);

	
	int index;
	for (int i = 0; i < width * height; ++i)
	{
		index = i * 3;
		B = data[index]; G = data[index + 1]; R = data[index + 2];
		data[index] = R; data[index + 1] = G; data[index + 2] = B;
	}

	
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);


	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	
	free(data);
	return 0;
}

struct color
{
	int r;
	int g;
	int b;

	color()
	{
		r = 0;
		b = 0;
		g = 0;
	}
	color(int r, int g, int b):r(r),g(g),b(b)
	{
	}
};

color black;
color white(255,255,255);

int textwidth(char* s)
{
	int width = 0;
	for (int i = 0; i < strlen(s); i++)
	{
		width += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

	}
	return width; 
}


class buttom
{
public:
	int width; 
	int height;
	int x; 
	int y;
	char text[20];
	color back;
	color textc;
	int textureid;

	buttom(int x, int y, int width, int height, const char* t, color back, color textc, int texture)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		strcpy_s(this->text, 20, t);
		this->back = back;
		this->textc = textc;
		this->textureid = texture;
	}

	void display()
	{
		glDisable(GL_COLOR);
		if (this->textureid > 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureid);
			glBegin(GL_QUADS);
			glTexCoord2f(textures[0][0], textures[0][1]);
			glVertex2d(x, y);
			glTexCoord2f(textures[1][0], textures[1][1]);
			glVertex2d(x+width, y);
			glTexCoord2f(textures[2][0], textures[2][1]);
			glVertex2d(x+width, y+height);
			glTexCoord2f(textures[3][0], textures[3][1]);
			glVertex2d(x, y + height);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glColor3ub(back.r, back.g, back.b);
			glBegin(GL_QUADS);
			glVertex2d(x, y);
			glVertex2d(x + width, y);
			glVertex2d(x + width, y + height);
			glVertex2d(x, y + height);
			glEnd();
		}
		glEnable(GL_COLOR);
		//textwidth(text);
		glColor3ub(textc.r,textc.g, textc.b);
		displayText(x + (width - textwidth(text))/2, y+(height)/2 - 12, text);
	}
	bool onClick()
	{
		if (x < xx && xx < x + width && y < yy && yy < y + height)
			return true;
		else
			return false; 
	}
};

class liner
{
public:
	int width;
	int height;
	int x;
	int y;
	color line;

	liner(int x, int y, int width, int height, color color)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->line = color;

	}
	void display()
	{
		glColor3ub(line.r, line.g, line.b);
		glBegin(GL_LINE_LOOP);
		glVertex2d(x, y);
		glVertex2d(x+width, y);
		glVertex2d(x+width, y+height);
		glVertex2d(x, y+height);
		glEnd();
	}
};

class target
{
public:
	shape* p;
	int m, n;
	char shapeType; // 't' - triangles, 's' - squares, 'd' - diamond   
	int currentDirection = 0; // 0 - right, 1 - up, 2 - left, 3 - down
	int health = 3;
};

int stageH = 1;
target* targets;

class tower
{
public:
	int m, n;
	const int r = 3;
	bool active = false;
};

tower* towers;
int nTargets = 0;
int nTowers = 0;

class bullet
{
public:
	int m, n;
	int iTarget;
	//int iTower;
	double angle;
	double r = 0;
	//bool active = true;
};
bullet* bullets;
//int nBullets = 0;

void gameOver(target* targets)
{
	if (targets[0].m == 19 && targets[0].n == 5)
	{
		status = 2;
	}
};
void displayText(float x, float y, const char* s) {
	int j = strlen(s);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
	}
}

void showTarget(target* t)
{
	if (t->health > 0)
	{
		switch (t->shapeType)
		{
		case ('t'):
			glColor3ub(239, 19, 19);
			glBegin(GL_POLYGON);
			glVertex2d(t->m * 40 + 5, t->n * 40 + 5);
			glVertex2d(t->m * 40 + 35, t->n * 40 + 5);
			glVertex2d(t->m * 40 + 20, t->n * 40 + 35);
			glEnd();
			break;
		}
	}
};

void showTowers(tower* towers)
{
	if(status != 3)
	{
		int i = 0;
		while (i < nTowers)
		{

			glDisable(GL_COLOR);
			glEnable(GL_TEXTURE_2D);
			switch (Tstatus) {
			case 1:
				glBindTexture(GL_TEXTURE_2D, 4);
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, 5);
				break;
			case 3:
				glBindTexture(GL_TEXTURE_2D, 6);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D, 4);
				break;
			}
			//glBindTexture(GL_TEXTURE_2D, 4);
			//glColor3ub(255, 255, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(textures[0][0], textures[0][1]);
			glVertex2d(towers[i].m * 40 + 5, towers[i].n * 40 + 5);
			glTexCoord2f(textures[1][0], textures[1][1]);
			glVertex2d(towers[i].m * 40 + 35, towers[i].n * 40 + 5);
			glTexCoord2f(textures[2][0], textures[2][1]);
			glVertex2d(towers[i].m * 40 + 35, towers[i].n * 40 + 35);
			glTexCoord2f(textures[3][0], textures[3][1]);
			glVertex2d(towers[i].m * 40 + 5, towers[i].n * 40 + 35);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR);

			//glColor3ub(20, 223, 54);
			//glBegin(GL_QUADS);
			//glTexCoord2f(textures[0][0], textures[0][1]);
			//glVertex2d(j * 40, i * 40);
			//glTexCoord2f(textures[1][0], textures[1][1]);
			//glVertex2d((j + 1) * 40, i * 40);
			//glTexCoord2f(textures[2][0], textures[2][1]);
			//glVertex2d((j + 1) * 40, (i + 1) * 40);
			//glTexCoord2f(textures[3][0], textures[3][1]);
			//glVertex2d(j * 40, (i + 1) * 40);
			//glEnd();
			//glDisable(GL_TEXTURE_2D);
			//glEnable(GL_COLOR);

			i++;


		}
	}
};

/*void circle(int x0, int y0, int r)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 36; i++)
	{
		glVertex2d(x0 + r * cos((i * M_PI) / 18), y0 + r * sin((i * M_PI) / 18));
	}
	glEnd();
}; */

void circle(int x0, int y0, int r, GLuint textureId)
{
	const int segments = 36;

	
	float textures[segments][2];

	for (int i = 0; i < segments; i++)
	{
		float angle = (float)i / (float)segments * 2.0f * M_PI;
		textures[i][0] = cos(angle) * 0.5f + 0.5f;
		textures[i][1] = sin(angle) * 0.5f + 0.5f;
	}


	glDisable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_POLYGON);
	for (int i = 0; i < segments; i++)
	{
		glTexCoord2f(textures[i][0], textures[i][1]);
		glVertex2d(x0 + r * cos((i * M_PI) / 18), y0 + r * sin((i * M_PI) / 18));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);
}

void showBullets(bullet* bullets)
{
	for (int i = 0; i < nTowers; i++)
	{
		//circle(bullets[i].m, bullets[i].n, 10);
		if (bullets[i].iTarget >= 0)
			circle(bullets[i].m, bullets[i].n, 10, 6);
	}
};

void skinPanel()
{
	buttom skin_panel(0, 80, 1080, 640, "", black, black, 9);
	skin_panel.display();
	
	glColor3ub(0, 0, 0);
	displayText(360, 610, "Choose which skin you want to try");
	displayText(460, 560, "Tower Skins");
	displayText(460, 380, "Field Skins");

};

buttom skin_T1_checker(260, 440, 80, 80, "", black, black, 7);
buttom skin_T2_checker(560, 440, 80, 80, "", black, black, 7);
buttom skin_T3_checker(860, 440, 80, 80, "", black, black, 7);


buttom skin_F1_checker(260, 260, 80, 80, "", black, black, 7);
buttom skin_F2_checker(560, 260, 80, 80, "", black, black, 7);
buttom skin_F3_checker(860, 260, 80, 80, "", black, black, 7);
void Skins()
{

	buttom skin_T1_holder(140, 440, 80, 80, "", black, black, 4);
	skin_T1_holder.display();

	buttom skin_T2_holder(440, 440, 80, 80, "", black, black, 5);
	skin_T2_holder.display();

	buttom skin_T3_holder(740, 440, 80, 80, "", black, black, 6);
	skin_T3_holder.display();
	
	if (Tstatus == 1)
		skin_T1_checker.textureid = 7;
	else skin_T1_checker.textureid = 8;
	skin_T1_checker.display();

	if (Tstatus == 2)
		skin_T2_checker.textureid = 7;
	else skin_T2_checker.textureid = 8;
	skin_T2_checker.display();

	if (Tstatus == 3)
		skin_T3_checker.textureid = 7;
	else skin_T3_checker.textureid = 8;
	skin_T3_checker.display();




	buttom skin_F1_holder(140, 260, 80, 80, "", black, black, 1);
	skin_F1_holder.display();

	if (Fstatus == 1)
	skin_F1_checker.textureid = 7;
	else skin_F1_checker.textureid = 8;
	skin_F1_checker.display();


	buttom skin_F2_holder(440, 260, 80, 80, "", black, black, 2);
	skin_F2_holder.display();

	if (Fstatus == 2)
		skin_F2_checker.textureid = 7;
	else skin_F2_checker.textureid = 8;
	skin_F2_checker.display();

	buttom skin_F3_holder(740, 260, 80, 80, "", black, black, 3);
	skin_F3_holder.display();

	if (Fstatus == 3)
		skin_F3_checker.textureid = 7;
	else skin_F3_checker.textureid = 8;
	skin_F3_checker.display();
	
 }
 buttom skins(1100, 260, 160, 80, "Skins", color(192, 192, 192), black, 0);
 buttom stop(1100, 140, 160, 80, "Stop", color(192, 192, 192), black, 0);
 buttom start(1100, 20, 160, 80, "Start", color(192, 192, 192), black, 0);
void showPanel()
{
	
	buttom bot_panel(0, 0, 1280, 80, " ", color(128, 128, 128), black, 0);
	bot_panel.display();
	liner bot_l(0, 0, 1280, 80, black);
	bot_l.display();

	buttom r_panel(1080, 0, 200, 720, "", color(128, 128, 128), black, 0);
	r_panel.display();
	liner r_l_panel(1080, 0, 200, 720, black);
	r_l_panel.display();
	
	stop.display();
	start.display();
	
	skins.display();
	if (status == 2)
	{
		glColor3ub(255, 255, 255);
		displayText(640, 460, "GameOver");
	}
};



void detection(tower* tw, target* tg, bullet* b)
{
	for (int i = 0; i < nTowers; i++)
	{
		for (int j = 0; j < nTargets; j++)
		{
			if (targets[j].health > 0)
			{

				if ((targets[j].m >= towers[i].m - towers[i].r + 1) && (targets[j].m <= towers[i].m + towers[i].r - 1)
					&& (targets[j].n >= towers[i].n - towers[i].r + 1) && (targets[j].n <= towers[i].n + towers[i].r - 1))
				{
					if (towers[i].active == false)//(nBullets == 0) //|| b[nBullets-1].iTarget != j)
					{
						b[i].n = towers[i].n * 40 + 20;
						b[i].m = towers[i].m * 40 + 20;
						b[i].iTarget = j;
						//b[i].iTower = i;
						double x = (targets[j].m - towers[i].m) * 40;
						double y = (targets[j].n - towers[i].n) * 40;
						b[i].r = 0;
						if (x == 0 && y > 0)
							b[i].angle = M_PI / 2;
						else if (x == 0 && y < 0)
							b[i].angle = -M_PI / 2;
						else if (x > 0 && y > 0)
							b[i].angle = atan2(y, x);
						else if (x < 0 && y < 0)
							b[i].angle = atan2(y, x);
						else
							b[i].angle = atan2(y, x);
						//nBullets++;
						towers[i].active = true;
					}
				}
			}
		}

	}
};

void miss(bullet* bullets, target* targets, tower* towers)
{
	for (int i = 0; i < nTowers; i++)
	{
		if (towers[i].active == true && towers[i].n > 1 && bullets[i].iTarget >= 0)
		{
			if ((bullets[i].m <= (towers[i].m * 40 + 20) - (towers[i].r * 40))
				|| (bullets[i].m >= (towers[i].m * 40 + 20) + (towers[i].r * 40))
				|| (bullets[i].n <= (towers[i].n * 40 + 20) - (towers[i].r * 40))
				|| (bullets[i].n >= (towers[i].n * 40 + 20) + (towers[i].r * 40)))
			{
				towers[i].active = false; // true = off, false = on
				bullets[i].iTarget = -1;
				cout << i << "miss" << endl;
			};
		}
	}
};

void hit(bullet* bullets, target* targets, tower* towers)
{
	int i = 0;
	while (i < nTowers)
	{
		int i0 = bullets[i].iTarget;
		//	double D = fabs(bullets[i].m - targets[bullets[i].iTarget].m * 40 + 20)
		//		+ fabs(bullets[i].n - targets[bullets[i].iTarget].n * 40 + 20);
	    if (bullets[i].iTarget < 0) { i++;continue;};
		if (towers[i].active == false) { i++; continue; };
		double D = ((bullets[i].m - targets[bullets[i].iTarget].m * 40 + 20)
			* (bullets[i].n - targets[bullets[i].iTarget].n * 40 + 20)) + ((bullets[i].m - targets[bullets[i].iTarget].m * 40 + 20)
				* (bullets[i].n - targets[bullets[i].iTarget].n * 40 + 20));
		if (sqrt(D) <= 30)
		{
			cout << "hit" << endl;
			bullets[i].iTarget = -1;
			targets[i0].health--;
			if (targets[i0].health == 0)
			{

				targets[i0].m = 0;
				targets[i0].n = 2;
				targets[i0].health = 3 * stageH;
				stageH += 30;
				//cout << stageH << endl;;
				//cout << targets[i0].health << endl;
				targets[i].currentDirection = 0;

			}
			towers[i].active = false;
			//for (int j = i; j < nBullets; j++)
			//{
			//	bullets[i] = bullets[i + 1];
			//}
			//nBullets--;

		}
		else i++;
	}
};
void inc(void)
{
	if (status != 1)
		return;

	// to do move next
	counterT += 2;

	if (counterT % 51 == 0)
	{
		for (int i = 0; i < nTargets; i++)
		{
			if (targets[i].health > 0)
			{
				if (targets[i].currentDirection == 0)
				{
					if (pRoad->a[targets[i].n][targets[i].m + 1] == 1)
					{

						targets[i].m++;
					}
					else if (pRoad->a[targets[i].n + 1][targets[i].m] == 1)
					{

						targets[i].n++;
						targets[i].currentDirection = 1;
					}
					else if (pRoad->a[targets[i].n - 1][targets[i].m] == 1)
					{

						targets[i].n--;
						targets[i].currentDirection = 3;
					}
					/*	else if (pRoad->a[targets[i].n][targets[i].m - 1] == 1)
						{

							targets[i].m--;
							targets[i].currentDirection = 2;
						} */
				}

				if (targets[i].currentDirection == 1)
				{
					if (pRoad->a[targets[i].n + 1][targets[i].m] == 1)
					{

						targets[i].n++;
					}
					else if (pRoad->a[targets[i].n][targets[i].m + 1] == 1)
					{

						targets[i].m++;
						targets[i].currentDirection = 0;
					}
					else if (pRoad->a[targets[i].n][targets[i].m - 1] == 1)
					{

						targets[i].m--;
						targets[i].currentDirection = 2;
					}
					/*	else if (pRoad->a[targets[i].n - 1][targets[i].m] == 1)
						{

							targets[i].n--;
							targets[i].currentDirection = 3;
						} */
				}

				if (targets[i].currentDirection == 2)
				{
					if (pRoad->a[targets[i].n][targets[i].m - 1] == 1)
					{

						targets[i].m--;
					}
					else if (pRoad->a[targets[i].n + 1][targets[i].m] == 1)
					{

						targets[i].n++;
						targets[i].currentDirection = 1;
					}
					else if (pRoad->a[targets[i].n - 1][targets[i].m] == 1)
					{

						targets[i].n--;
						targets[i].currentDirection = 3;
					}
					else if (pRoad->a[targets[i].n][targets[i].m + 1] == 1)
					{

						targets[i].m++;
						targets[i].currentDirection = 0;
					}
				}

				if (targets[i].currentDirection == 3)
				{
					if (pRoad->a[targets[i].n - 1][targets[i].m] == 1)
					{

						targets[i].n--;
					}
					else if (pRoad->a[targets[i].n][targets[i].m + 1] == 1)
					{

						targets[i].m++;
						targets[i].currentDirection = 0;
					}

					else if (pRoad->a[targets[i].n][targets[i].m - 1] == 1)
					{

						targets[i].m--;
						targets[i].currentDirection = 2;
					}
					/*	else if (pRoad->a[targets[i].n + 1][targets[i].m] == 1)
					   {

						   targets[i].n++;
						   targets[i].currentDirection = 1;
					   } */
				}
			}
		}
	}

	detection(towers, targets, bullets);

	for (int i = 0; i < nTowers; i++)
	{
		bullets[i].r += 1, 5;
		bullets[i].m = towers[i].m * 40 + 20 + bullets[i].r * cos(bullets[i].angle);
		bullets[i].n = towers[i].n * 40 + 20 + bullets[i].r * sin(bullets[i].angle);

	};
	//if (counterT % 25 == 0)
	hit(bullets, targets, towers);
	miss(bullets, targets, towers);
	gameOver(targets);
	glutPostRedisplay();

};

int arr[18][32] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
					{0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
					{0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
					{0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					{0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
					{0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

void createTileRoad(int i, int j)
{
	//glColor3ub(255, 255, 255);
	//glBegin(GL_POLYGON);
	//glVertex2d(j * 40, i * 40);
	//glVertex2d((j + 1) * 40, i * 40);
	//glVertex2d((j + 1) * 40, (i + 1) * 40);
	//glVertex2d(j * 40, (i + 1) * 40);
	//glEnd();

	glDisable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 10);
	glBegin(GL_QUADS);
	glTexCoord2f(textures[0][0], textures[0][1]);
	glVertex2d(j * 40, i * 40);
	glTexCoord2f(textures[1][0], textures[1][1]);
	glVertex2d((j + 1) * 40, i * 40);
	glTexCoord2f(textures[2][0], textures[2][1]);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glTexCoord2f(textures[3][0], textures[3][1]);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(j * 40, i * 40);
	glVertex2d((j + 1) * 40, i * 40);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd();

	/*glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(j * 40, i * 40);
	glVertex2d((j + 1) * 40, i * 40);
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d((j + 1) * 40, i * 40);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(j * 40, i * 40);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd(); */
};

void createTileField(int i, int j)
{
	glDisable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	switch (Fstatus) {
	case 1:
		glBindTexture(GL_TEXTURE_2D, 1);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D, 2);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_2D, 3);
		break;
	default:
		glBindTexture(GL_TEXTURE_2D, 1);
		break;
	}
	//glBindTexture(GL_TEXTURE_2D, 1);

	//glColor3ub(20, 223, 54);
	glBegin(GL_QUADS);
	glTexCoord2f(textures[0][0], textures[0][1]);
	glVertex2d(j * 40, i * 40);
	glTexCoord2f(textures[1][0], textures[1][1]);
	glVertex2d((j + 1) * 40, i * 40);
	glTexCoord2f(textures[2][0], textures[2][1]);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glTexCoord2f(textures[3][0], textures[3][1]);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(j * 40, i * 40);
	glVertex2d((j + 1) * 40, i * 40);
	glVertex2d((j + 1) * 40, (i + 1) * 40);
	glVertex2d(j * 40, (i + 1) * 40);
	glEnd();
	
};

void init()
{
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	char buf[80];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// play zone 1280x720 green 
	// tile amount N = 32, M = 18, 40px tile side
	//glColor3ub(20, 223, 54);
	//glBegin(GL_POLYGON);
	//glVertex2d(0, 0);
	//glVertex2d(w, 0);
	//glVertex2d(w, h);
	//glVertex2d(0, h);
	//glEnd();

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (pRoad->a[i][j] == 1)
				createTileRoad(i, j);
			else createTileField(i, j);
		}

	};

	showBullets(bullets);
	showTarget(targets);
	
	if (status == 3)
	{
		skinPanel();
		Skins();
	}
	showPanel();
	showTowers(towers);

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			xx = x;
			yy = h - y;
			int im, jm;
			im = xx / 40;
			jm = yy / 40;
			if (stop.onClick()) //stop
			{
				status = 0;
				glutPostRedisplay();
			}
		
			if (start.onClick()) // start
			{
				status = 1;
				glutPostRedisplay();
			}
			for (int i = 0; i < nTowers; i++)
			{
				if (im == towers[i].m && jm == towers[i].n)
				{
					currentTower = i;

					glutPostRedisplay();
				}
				else
				{
					
					ButtonStatus = 1;
					//	currentTower = -1;

				}
				if (skins.onClick()) // skins bottom
				{
					status = 3;
					glutPostRedisplay();
				}
				if (status == 3)
				{
					if (skin_T1_checker.onClick())
						Tstatus = 1;
					if (skin_T2_checker.onClick())
						Tstatus = 2;
					if (skin_T3_checker.onClick())
						Tstatus = 3;

					if (skin_F1_checker.onClick())
						Fstatus = 1;
					if (skin_F2_checker.onClick())
						Fstatus = 2;
					if (skin_F3_checker.onClick())
						Fstatus = 3;
				}
			}
		}
		else
			if (state == GLUT_UP)
			{
				if (currentTower != -1)
				{
					if (pRoad->a[towers[currentTower].n][towers[currentTower].m] == 1 || status > 0 || towers[currentTower].m > 26)
					{
						towers[currentTower].m = currentTower;
						towers[currentTower].n = 1;
						towers[currentTower].active = true;
					}
					for (int i = 0; i < nTowers; i++)
					{
						if ((towers[currentTower].m == towers[i].m && towers[currentTower].n == towers[i].n && currentTower != i))
						{

							towers[currentTower].m = currentTower;
							towers[currentTower].n = 1;
							towers[currentTower].active = true;
							break;
						}
					};
				}
				
				ButtonStatus = -1;
				currentTower = -1;
			}
		break;
	default:
		break;
	}
}
void MouseMotion(int x, int y)
{
	if (ButtonStatus == 1)
	{

		int xx, yy;
		xx = x;
		yy = h - y;
		int im, jm;
		im = xx / 40;
		jm = yy / 40;
		if (currentTower != -1)
		{
			towers[currentTower].m = im;
			towers[currentTower].n = jm;
			towers[currentTower].active = false;
		}
		glutPostRedisplay();
	}
}
void keyb(unsigned char key, int x, int y)
{
	const unsigned char esc = 27;
	if (key == esc)
	{	
		delete[] towers;
		delete[] bullets;
		delete targets->p;
		delete targets; 
		delete pRoad; 
		exit(0);
	}
}

void reshape(int w1, int h1)
{
	glViewport(0, 0, w1, h1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	pRoad = new matrix<int>(N, M);
	//std::unique_ptr<matrix<int>> pRoad(N, M);


	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			pRoad->a[i][j] = arr[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << pRoad->a[i][j] << " ";
		}
		cout << endl;
	}

	targets = new target;
	nTargets++;
	//((triangle*)targets[0])->setPoints(5, 5, 35, 5, 20, 20);
	//((triangle*)targets)->setPoint();
	targets->p = new triangle;
	targets->shapeType = 't';
	//((triangle*)targets->p)->setPoint();
	targets->n = 2;
	targets->m = 0;

	towers = new tower[10];
	//std::shared_ptr<tower[]> towers(new tower[10]);
	/*
	towers[nTowers].m = 4;
	towers[nTowers].n = 7;
	nTowers++;
	towers[nTowers].m = 4;
	towers[nTowers].n = 13;
	nTowers++;
	towers[nTowers].m = 10;
	towers[nTowers].n = 10;
	nTowers++;
	*/

	for (int i = 0; i < 10; i++)
	{
		towers[nTowers].m = i;
		towers[nTowers].n = 1;
		towers[nTowers].active = true;
		nTowers++;
	}
	bullets = new bullet[10];

	for (int i = 0; i < 10; i++)
	{
		bullets[i].iTarget = -1;
	}

	glutInit(&argc, argv);



	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);



	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Game TD");


	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	LoadTexture(1, "Field1.bmp", 15, 15);
	LoadTexture(2, "Field2.bmp", 15, 15);
	LoadTexture(3, "Field3.bmp", 16, 16);
	LoadTexture(4, "Tower1.bmp", 16, 16);
	LoadTexture(5, "Tower2.bmp", 16, 16);
	LoadTexture(6, "Tower3.bmp", 16, 16);
	LoadTexture(7, "True.bmp", 16, 16);
	LoadTexture(8, "False.bmp", 16, 16);
	LoadTexture(9, "frame.bmp", 1216, 1216);
	LoadTexture(10, "stone.bmp", 16, 16);
	glEnable(GL_CULL_FACE);

	init();

	glutDisplayFunc(display);


	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(keyb);

	glutIdleFunc(inc);



	glutMainLoop();


	return 0;
}


