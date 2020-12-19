#pragma once
//переменные для работы с движением 
#include "MyVector3d.h"
#include "SkateSystem.h"
#include "MyOGL.h"
#include <ctime>
#include "ObjLoader.h"
#include "Texture.h"
extern SkateСontrolSystem Skate;
//время последнего кадра
extern double TimeLastFrame;

extern ObjFile skateboardObj;
extern Texture skateboardTex;

//рассчет новой точки
void NewPositionSkate() {
	if (Skate.KeyDuration_W > 6) {
		Skate.KeyDuration_W = 6;
	}
	Skate.SkatePosition.setCoord_X(Skate.SkatePosition.X() + Skate.Speed * cos(Skate.AngleZ * PI / 180) * (Skate.KeyDuration_W / 5));
	Skate.SkatePosition.setCoord_Y(Skate.SkatePosition.Y() + Skate.Speed * sin(Skate.AngleZ * PI / 180) * (Skate.KeyDuration_W / 5));

}

//обработчик нажания кнопок 
void UpdateParameters() {
	//получаем текущее время
	double end = clock();

	//время кадра
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//замедление объекта при движении вперед, если не нажаты клавиши W и S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W > 0) {
		//проверяем, чтоб время нажетия W не ушло в минус
		if (Skate.KeyDuration_W - TimeFrame < 0) {
			Skate.KeyDuration_W = 0;
		}
		else
		{
			Skate.KeyDuration_W -= TimeFrame;
			NewPositionSkate();
		}
		//устанавливаем флаг на разгон после торможения
		Skate.FlagAccAfterDec = true;
	}

	//плавное замедление при движении назад
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

	//торможение при движении вперед, нажата только S
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

	//торможение при движении назад
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

	//движение вперед, нажата только W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Skate.KeyDuration_W >= 0)
	{
		//проверяем, есть ли сейчас разгон
		if (!Skate.FlagAcceleration)
		{
			Skate.FlagAcceleration = true;
		}
		else
		{
			//ускоряемся
			Skate.KeyDuration_W += TimeFrame;
			NewPositionSkate();
		}
	}

	//движение назад, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Skate.KeyDuration_W <= 0)
	{
		if (!Skate.FlagDeceleration)
		{
			Skate.FlagDeceleration = true;
		}
		else
		{
			//замедляемся
			Skate.KeyDuration_W -= TimeFrame;
			NewPositionSkate();
		}
	}

	//поворачиваем вправо
	if (OpenGL::isKeyPressed('D')) {
		
		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Skate.AngleZ -= Skate.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Skate.AngleZ += Skate.SpeedWheel / 10;
		}
	}

	//поворачиваем влево
	if (OpenGL::isKeyPressed('A')) {
		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Skate.AngleZ += Skate.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Skate.AngleZ -= Skate.SpeedWheel / 10;
		}
	}

	
	//P - вернуть в исходное положение
	if (OpenGL::isKeyPressed('P')) {
		Skate = SkateСontrolSystem();
	}

	//пробел стоп
	if (OpenGL::isKeyPressed(32)) {
		Skate.KeyDuration_W = 0;
	}
}

void SkateSystem() {
	//обновление параметров 
	UpdateParameters();

	//обновление времени последнего кадра
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