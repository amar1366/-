#include "DrawBezierAndHermit.h"
#include "ObjLoader.h"
#include <random>
#include <iostream>
#include "SkateSystem.h"
#include "Texture.h"
#include "MyOGL.h"

using namespace std;

//время последнего кадра
extern double TimeLastFrame;
extern SkateСontrolSystem Skate;
extern ObjFile manObj, planetObj, skateboardObj;
extern Texture planetTex, skateboardTex, skyTex, fireTex;
extern bool Restart;


vector<Point> Position = vector<Point>();
bool man_skate = false;
int man_flag = -1;

double a = -40;
double b = 40;

int man = 10;

vector<vector<Point>> Pos_planet = vector<vector<Point>>();


double random(int a, int b)
{
	std::random_device random_device; // Источник энтропии.
	std::mt19937 generator(random_device()); // Генератор случайных чисел.
	// (Здесь берется одно инициализирующее значение, можно брать больше)

	std::uniform_int_distribution<> distribution(a, b); // Равномерное распределение [a, b]

	return distribution(generator); // Случайное число.
}

vector<float> Color_3d() {
	vector<float> color = {
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000
	};
	return color;
}

//новое местополежение человечков
void NewPosition() {
	double x_min = -10;
	double x_max = 10;
	double y_min = -10;
	double y_max = 10;

	Position.clear();
	Pos_planet.clear();
	for (int i = 0; i < man; i++) {
		//кривые безье для планет
		vector<Point> points = {
			Point(random(a, b), random(a, b), random(a, b)),
			Point(random(a, b), random(a, b), random(a, b)),
			Point(random(a, b), random(a, b), random(a, b)),
			Point(random(a, b), random(a, b), random(a, b))
		};
		Pos_planet.push_back(points);

		//позции, цвета, флаги переноса для человечков
		Position.push_back(Point(random(x_min, x_max), random(y_min, y_max), 0));
	}
}

void com() {
	//поставить человечка
	if (OpenGL::isKeyPressed('C') && man_flag != -1 && man_skate) {
		//скейт свободен
		man_skate = false;

		Position[man_flag].x = Skate.SkatePosition.X() * 0.5;
		Position[man_flag].y = Skate.SkatePosition.Y() * 0.5;
		Position[man_flag].z = 0;
	}
}

void SkatePlay() {
	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	//перезапуск
	if (Restart == true) {
		Skate = SkateСontrolSystem();
		NewPosition();
		Restart = false;
	}


	//проверяем нажатие клавишь
	com();

	//анализ ситуации на поле
	for (int i = 0; i < (int)Position.size(); i++) {
		double R_man = 0.4;
		double R_Skate = 1.0;

		//пересечении скейта с человечком
		double CD = sqrt(pow(Skate.SkatePosition.X() * 0.5 - Position[i].x, 2) + pow(Skate.SkatePosition.Y() * 0.5 - Position[i].y, 2)) - R_man - R_Skate;
		if (CD <= 0) {
			if (man_skate == false && man_flag != i) {
				man_flag = i;
				man_skate = true;
			}
		}
	}

	//когда скейт подхватил человечка
	if (man_skate) {
		Position[man_flag].x = Skate.SkatePosition.X() * 0.5;
		Position[man_flag].y = Skate.SkatePosition.Y() * 0.5;
		Position[man_flag].z = Skate.SkatePosition.Z() * 0.5 + 0.4;
	}


	//рисуем человечков
	for (int i = 0; i < (int)Position.size(); i++) {

		glPushMatrix();
		Position[i].Translated();
		fireTex.bindTexture();
		manObj.DrawObj();
		glPopMatrix();
	}


	//угол поворота планеты
	static double angle = 0;

	//настройка времени
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max становится = 1 за 5 секунд
	//t_max сама по себе изменяется от 0 до 1 постепенно от кадра к кадру
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max = 0;
		t_max_next = 0;
		flag_tmax = true;

		//угол поворота планеты
		angle = 0;

		//кривые безье для планет
		Pos_planet.clear();
		for (int i = 0; i < man; i++) {
			vector<Point> points = {
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b))
			};
			Pos_planet.push_back(points);
		}
	}

	angle += t_max * 0.4 * 40;

	for (int i = 0; i < (int)Pos_planet.size(); i++) {
		glPushMatrix();

		Point point = BezierCurve3(Pos_planet[i][0], Pos_planet[i][1], Pos_planet[i][2], Pos_planet[i][3], t_max);
		Point next_point = BezierCurve3(Pos_planet[i][0], Pos_planet[i][1], Pos_planet[i][2], Pos_planet[i][3], t_max_next);
		glScaled(2, 2, 2);
		point.Translated();
		glRotated(angle, 0, 1, 0);
		planetTex.bindTexture();
		planetObj.DrawObj();

		glPopMatrix();
	}
}