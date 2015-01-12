#pragma once


#include"Setting.h"
#include"HandRecognition.h"

class HandMouse{
public:
	HandMouse(HandRecognition*);
	~HandMouse();

private:
	POINT *privious_points;
	int next_index;
	POINT mouse_point;

	HandRecognition* hand;

public:
	const void moveMouse();
	void setNextPoint(int,int);

};