#pragma once
#include "MyVector3d.h"
class SkateСontrolSystem {
public:
	//максимальный угол поворота 
	double MaxAngleWheel = 27;

	//угол поворота объекта относительно оси Z
	double AngleZ = 0;

	//угол попорота 
	double AngleWheel = 0;

	//скорость 
	double Speed = 0.1;

	//скорость вращения 
	double SpeedWheel = 4;

	//скорость торможенияw
	double SpeedSlow = 0.1;

	//период нажатия W
	double KeyDuration_W = 0;

	//Происходит ли сейчас торможение
	bool FlagDeceleration = false;

	//происходит ли сейчас разгон
	bool FlagAcceleration = false;

	//Разгон после торможения
	bool FlagAccAfterDec = false;

	//позиция объекта
	Vector3 SkatePosition;

	bool Stop = false;

	SkateСontrolSystem() {
		SkatePosition = Vector3(0, 0, 0);
	}
};