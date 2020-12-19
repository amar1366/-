#pragma once
#include "MyVector3d.h"
class Skate�ontrolSystem {
public:
	//������������ ���� �������� 
	double MaxAngleWheel = 27;

	//���� �������� ������� ������������ ��� Z
	double AngleZ = 0;

	//���� �������� 
	double AngleWheel = 0;

	//�������� 
	double Speed = 0.1;

	//�������� �������� 
	double SpeedWheel = 4;

	//�������� ����������w
	double SpeedSlow = 0.1;

	//������ ������� W
	double KeyDuration_W = 0;

	//���������� �� ������ ����������
	bool FlagDeceleration = false;

	//���������� �� ������ ������
	bool FlagAcceleration = false;

	//������ ����� ����������
	bool FlagAccAfterDec = false;

	//������� �������
	Vector3 SkatePosition;

	bool Stop = false;

	Skate�ontrolSystem() {
		SkatePosition = Vector3(0, 0, 0);
	}
};