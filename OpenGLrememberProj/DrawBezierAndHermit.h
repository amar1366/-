#pragma once
#define _USE_MATH_DEFINES // for C
#include <Windows.h>
#include <chrono>
#include <vector>

#include"Draw.h"
#include <gl\GL.h>
#include "Point.h"
#include "Draw.h"


//включение/отключение рисование линий
bool line = false;
double sharpness_t = 0.01;


//Рассчет одной точки для кривой Эрмита
Point HermitCurve(Point P1, Point R1, Point P2, Point R2, double t) {
	double t_2 = t * t;
	double t_3 = t * t * t;

	double x = P1.x * (2 * t_3 - 3 * t_2 + 1) + P2.x * (-2 * t_3 + 3 * t_2) + R1.x * (t_3 - 2 * t_2 + t) + R2.x * (t_3 - t_2);
	double y = P1.y * (2 * t_3 - 3 * t_2 + 1) + P2.y * (-2 * t_3 + 3 * t_2) + R1.y * (t_3 - 2 * t_2 + t) + R2.y * (t_3 - t_2);
	double z = P1.z * (2 * t_3 - 3 * t_2 + 1) + P2.z * (-2 * t_3 + 3 * t_2) + R1.z * (t_3 - 2 * t_2 + t) + R2.z * (t_3 - t_2);

	return Point(x, y, z);
}

//Рассчитать точки кривой Эрмита
//P1 - точка начала кривой, P2 - точка конца кривой
//R1 - вектор произвольной кривой при t = 0, R2 - вектор проивольной кривой при t = 1
//t - шаг
//t_max - максимальное значение t
vector<Point> PointHermitCurve(Point P1, Point R1, Point P2, Point R2, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(HermitCurve(P1, R1, P2, R2, i));
	}
	return points;
}

//Рассет одной точки для кривой Безье второго порядка
Point BezierCurve2(Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double var = 1 - t;
	double var_2 = var * var;

	double x = P1.x * var_2 + 2 * P2.x * t * var + P3.x * t_2;
	double y = P1.y * var_2 + 2 * P2.y * t * var + P3.y * t_2;
	double z = P1.z * var_2 + 2 * P2.z * t * var + P3.z * t_2;

	return Point(x, y, z);
}

//Рассчитать точки кривой Безье второго порядка
//P1, P2, P3 - точки, t - шаг, t_max - максимальное значение t 
vector<Point> PointBezierCurve2(Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve2(P1, P2, P3, i));
	}
	return points;
}

//Рассчитать точки кривой Безье третьего порядка
Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double t_3 = t_2 * t;
	double var = 1 - t;
	double var_2 = var * var;
	double var_3 = var_2 * var;

	double x = var_3 * P0.x + 3 * t * var_2 * P1.x + 3 * t_2 * var * P2.x + t_3 * P3.x;
	double y = var_3 * P0.y + 3 * t * var_2 * P1.y + 3 * t_2 * var * P2.y + t_3 * P3.y;
	double z = var_3 * P0.z + 3 * t * var_2 * P1.z + 3 * t_2 * var * P2.z + t_3 * P3.z;

	return Point(x, y, z);
}

//Рассчитать точки кривой Безье третьего порядка
//Начинается в точке P0, стремится приблизится к P1, затем к P2 и заканчивается в точке P3
//t - шаг, t_max - максимальное значение t
vector<Point> PointBezierCurve3(Point P0, Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve3(P0, P1, P2, P3, i));
	}
	return points;
}


//найти определитель матрицы 3 на 3
double SearchDeterminantMatrix3(double m[3][3]) {
	return m[0][0] * m[1][1] * m[2][2] +
		m[2][0] * m[0][1] * m[1][2] +
		m[1][0] * m[2][1] * m[0][2] -
		m[2][0] * m[1][1] * m[0][2] -
		m[0][0] * m[2][1] * m[1][2] -
		m[1][0] * m[0][1] * m[2][2];
}


enum Method {
	Hermit,
	Bezier2,
	Bezier3
};

//поворот фигуры
void MoveAxis(Point point, Point next_point) {
	Point dir = Draw::SearchVector(point, next_point);
	dir = Draw::VectorNormalization(dir);

	Point orig(1, 0, 0);
	Point rotX(dir.x, dir.y, 0);
	rotX = Draw::VectorNormalization(rotX);
	double cosU = Draw::ScalarProduct(orig, rotX);
	Point vecpr = Draw::VectorProduct(orig, rotX);

	double sinSign = vecpr.z / abs(vecpr.z);
	double U = acos(cosU) * 180.0 / M_PI * sinSign;
	double ZU = acos(dir.z) * 180.0 / M_PI - 90;

	glRotated(U, 0, 0, 1);
	glRotated(ZU, 0, 1, 0);

}

//point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max);
//next_point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max_next);
//point.Translated();
//MoveAxis(point, next_point);

//факториал
//Начало функции нахождения факториала
int factorial(int N)
{
	if (N < 0)
		return 0;
	if (N == 0)
		return 1;
	else
		return N * factorial(N - 1);
}

//функция базиса Бернштейна
double FunctionBernsteinBasis(int n, int i, double t) {
	return (factorial(n) / (factorial(n - i) * factorial(i))) * pow(t, i) * pow(1 - t, n - i);
}

//определение радиус вектора
Point FunctionBezierSurface(vector<vector<Point>> mas, double u, double v) {
	Point R(0, 0, 0);
	int row = (int)mas.size();
	for (int i = 0; i < row; i++) {
		int col = (int)mas[i].size();
		for (int j = 0; j < col; j++) {
			double Bi = FunctionBernsteinBasis(row - 1, i, u);
			double Bj = FunctionBernsteinBasis(col - 1, j, v);
			R.x += Bj * Bi * mas[i][j].x;
			R.y += Bj * Bi * mas[i][j].y;
			R.z += Bj * Bi * mas[i][j].z;
		}
	}
	return R;
}


//измеряем промежутки времени между отрисовкой
double Search_delta_time() {
	static auto end_render = std::chrono::steady_clock::now();
	auto cur_time = std::chrono::steady_clock::now();
	auto deltatime = cur_time - end_render;
	double delta = 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count() / 1000000;
	end_render = cur_time;
	return delta;
}
