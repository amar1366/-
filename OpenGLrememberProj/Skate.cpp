#pragma once
//���������� ��� ������ � ��������� 
#include "MyVector3d.h"
#include "SkateSystem.h"
#include "MyOGL.h"
#include <ctime>
#include "ObjLoader.h"
#include "Texture.h"
extern Skate�ontrolSystem Skate;
//����� ���������� �����
extern double TimeLastFrame;

extern ObjFile skateboardObj;
extern Texture skateboardTex;

//������� ����� �����
void NewPositionSkate() {
	if (Skate.KeyDuration_W > 6) {
		Skate.KeyDuration_W = 6;
	}
	Skate.SkatePosition.setCoord_X(Skate.SkatePosition.X() + Skate.Speed * cos(Skate.AngleZ * PI / 180) * (Skate.KeyDuration_W / 5));
	Skate.SkatePosition.setCoord_Y(Skate.SkatePosition.Y() + Skate.Speed * sin(Skate.AngleZ * PI / 180) * (Skate.KeyDuration_W / 5));

}

//���������� ������� ������ 
void UpdateParameters() {
	//�������� ������� �����
	double end = clock();

	//����� �����
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//���������� ������� ��� �������� ������, ���� �� ������ ������� W � S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W > 0) {
		//���������, ���� ����� ������� W �� ���� � �����
		if (Skate.KeyDuration_W - TimeFrame < 0) {
			Skate.KeyDuration_W = 0;
		}
		else
		{
			Skate.KeyDuration_W -= TimeFrame;
			NewPositionSkate();
		}
		//������������� ���� �� ������ ����� ����������
		Skate.FlagAccAfterDec = true;
	}

	//������� ���������� ��� �������� �����
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W < 0)
	{
		if (Skate.KeyDuration_W + TimeFrame > 0)
			Skate.KeyDuration_W = 0;
		else
		{
			Skate.KeyDuration_W += TimeFrame;
			NewPositionSkate();
		}
	}

	//���������� ��� �������� ������, ������ ������ S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Skate.KeyDuration_W > 0)
	{
		if (Skate.KeyDuration_W - Skate.SpeedSlow < 0)
			Skate.KeyDuration_W = 0;
		else
		{
			Skate.KeyDuration_W -= Skate.SpeedSlow;
			NewPositionSkate();
		}

	}

	//���������� ��� �������� �����
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W < 0)
	{
		if (Skate.KeyDuration_W + Skate.SpeedSlow < 0)
			Skate.KeyDuration_W = 0;
		else
		{
			Skate.KeyDuration_W += Skate.SpeedSlow;
			NewPositionSkate();
		}
	}

	//�������� ������, ������ ������ W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W >= 0)
	{
		//���������, ���� �� ������ ������
		if (!Skate.FlagAcceleration)
		{
			Skate.FlagAcceleration = true;
		}
		else
		{
			//����������
			Skate.KeyDuration_W += TimeFrame;
			NewPositionSkate();
		}
	}

	//�������� �����, ������ ������ S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Skate.KeyDuration_W <= 0)
	{
		if (!Skate.FlagDeceleration)
		{
			Skate.FlagDeceleration = true;
		}
		else
		{
			//�����������
			Skate.KeyDuration_W -= TimeFrame;
			NewPositionSkate();
		}
	}

	//������������ ������
	if (OpenGL::isKeyPressed('D')) {
		
		//����������� ������ ��������
		if (OpenGL::isKeyPressed('W')) {
			Skate.AngleZ -= Skate.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Skate.AngleZ += Skate.SpeedWheel / 10;
		}
	}

	//������������ �����
	if (OpenGL::isKeyPressed('A')) {
		//����������� ������ ��������
		if (OpenGL::isKeyPressed('W')) {
			Skate.AngleZ += Skate.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Skate.AngleZ -= Skate.SpeedWheel / 10;
		}
	}

	
	//P - ������� � �������� ���������
	if (OpenGL::isKeyPressed('P')) {
		Skate = Skate�ontrolSystem();
	}

	//������ ����
	if (OpenGL::isKeyPressed(32)) {
		Skate.KeyDuration_W = 0;
	}
}

void SkateSystem() {
	//���������� ���������� 
	UpdateParameters();

	//���������� ������� ���������� �����
	TimeLastFrame = clock();

	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(Skate.SkatePosition.X(), Skate.SkatePosition.Y(), Skate.SkatePosition.Z() + 0.2);
	glRotated(90, 0, 0, 1);
	glRotated(Skate.AngleZ, 0, 0, 1);
	
	skateboardTex.bindTexture();
	skateboardObj.DrawObj();
	
	glPopMatrix();
}